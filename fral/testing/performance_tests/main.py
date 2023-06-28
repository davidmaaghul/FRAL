# Run from project directory
import os.path
import argparse
import os
import time

import pandas as pd
import trio

from fral.testing.performance_tests import TEST_PATH, BLOB_SIZES, ENTRIES
from fral.testing.performance_tests.vm_client import VM

from typing import Dict
import json

NET_PORT = 50051
LINUX_PERFORMANCE = [
    'sudo sysctl net.core.default_qdisc=fq',
    'sudo sysctl net.ipv4.tcp_congestion_control=bbr',
    'sudo sysctl vm.swappiness=10',
    'sudo sysctl vm.drop_caches=3',
    'sudo sh -c "echo always > /sys/kernel/mm/transparent_hugepage/enabled"',
    'sudo sh -c "echo always > /sys/kernel/mm/transparent_hugepage/defrag"',
    'sudo sysctl kernel.numa_balancing=0',
]


def read_config(cfg_path: str) -> Dict:
    with open(cfg_path, "r") as f:
        return json.load(f)


async def build_cpp(vm: VM):
    build_script = os.path.join(TEST_PATH, "build.sh")
    await vm.cmd(f"chmod +x {build_script}")  # in case of pull
    await vm.cmd(f"{build_script}")


async def main():

    args = get_args()
    cfg = read_config(args.cfg_file)
    wb = cfg['working-branch']
    vm_wd = cfg['working-directory']

    vm_results_path = f'{TEST_PATH}/test-results'

    os.system(f'git commit -am "autocommit"; git push origin {wb}')

    if not os.path.exists(local_files := f'{vm_results_path}/vm'):
        os.makedirs(local_files)

    vm1 = await VM(cfg['vm1'], vm_wd, wb, name='vm1').connect()

    await vm1.cmd(f"rm -r {vm_results_path}")
    await vm1.cmd(f"mkdir -p {vm_results_path}/local")
    await build_cpp(vm1)
    await vm1.cmd(LINUX_PERFORMANCE)

    if args.py_test:
        await vm1.cmd(f"python3 {TEST_PATH}/python_tests.py")

    if args.write_test:
        await vm1.cmd(f"python3 {TEST_PATH}/write_test.py")

    if args.pc_test:
        await vm1.cmd(f"python3 {TEST_PATH}/pc_test.py")

    if args.net_test:
        vm2 = await VM(cfg['vm2'], vm_wd, wb, name='vm2').connect()
        await build_cpp(vm2)
        await vm2.cmd(LINUX_PERFORMANCE)

        for blob_size in BLOB_SIZES:
            for entries in ENTRIES:
                await vm1.cmd(f"kill -9 $(lsof -t -i:{NET_PORT})")  # kill previous tunnel
                await vm2.tunnel(vm1, NET_PORT)

                async with trio.open_nursery() as nursery:
                    nursery.start_soon(vm1.cmd, f"python3 {TEST_PATH}/server.py -b {blob_size} -e {entries}")
                    nursery.start_soon(vm2.cmd, f"python3 {TEST_PATH}/client.py -b {blob_size} -e {entries}")
                time.sleep(5)
    await vm1.file_transfer(vm_results_path)

    if args.net_test:
        df = None
        for f in os.listdir(test_files := os.path.join(vm_results_path, "vm", "local")):
            if "net_test" in f and f != "net_test.csv":
                sub_df = pd.read_csv(abs_f := os.path.join(test_files, f))
                df = pd.concat([df, sub_df]) if isinstance(df, pd.DataFrame) else sub_df
                os.remove(abs_f)

        df.columns = [col.strip() for col in df.columns]
        df.to_csv(os.path.join(vm_results_path, "vm", "local", "net_test.csv"), index=False, compression='gzip')


def get_args():
    arg_parser = argparse.ArgumentParser()
    arg_parser.add_argument(
        "-w", "--write_test", dest="write_test", action="store_true", default=False
    )
    arg_parser.add_argument(
        "-p", "--pc_test", dest="pc_test", action="store_true", default=False
    )
    arg_parser.add_argument(
        "-t", "--py_test", dest="py_test", action="store_true", default=False
    )
    arg_parser.add_argument(
        "-n", "--net_test", dest="net_test", action="store_true", default=False
    )
    arg_parser.add_argument(
        "-c", "--cfg_file", dest="cfg_file", nargs='?', default=os.path.join(TEST_PATH, "test_cfg.json")
    )

    return arg_parser.parse_args()


if __name__ == "__main__":
    trio.run(main)
