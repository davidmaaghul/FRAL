import logging
import sys
import trio
from typing import Dict, List
from subprocess import PIPE, STDOUT


class VM(object):
    def __init__(
        self,
        vm_cfg: Dict,
        wd: str,
        wb: str,
        end_marker: str = "--~~--END--~~--",
        name: str = "",
    ):
        self.username = vm_cfg["username"]
        self.host = vm_cfg["host"]
        self._local_key = vm_cfg["local-key"]
        self._remote_key = vm_cfg.get("remote-key")
        self._name = name if name else vm_cfg["name"]
        self._end_marker = end_marker
        self._working_directory = wd
        self._working_branch = wb
        self.process = None
        self._logger = logging.getLogger(self._name)
        self._setup_logger()

    def _setup_logger(self):
        self._logger.setLevel(logging.INFO)
        handler = logging.StreamHandler(sys.stdout)
        handler.setLevel(logging.INFO)
        handler.setFormatter(
            logging.Formatter(
                fmt=f"{self._name}: [%(asctime)s] %(levelname)s: %(message)s",
            )
        )
        self._logger.addHandler(handler)

    async def _write_close(self):
        await self.process.stdin.send_all(f"echo {self._end_marker}\n".encode())

    async def _read(self):
        b_line = await self.process.stdout.receive_some()
        return b_line.decode()

    async def _read_all(self):
        await self._write_close()
        read = ""
        while True:
            line = await self._read()
            # caveat is end marker must be a unique string
            if (end := line.find(self._end_marker)) != -1:
                read += line[:end]
                break
            read += line
        if read:
            self._logger.info(read.rstrip("\n"))

    async def _execute(self, cmd_str: str):
        await self.process.stdin.send_all(f"echo $ {cmd_str}\n".encode())
        await self._read_all()
        await self.process.stdin.send_all(f"{cmd_str}\n".encode())
        await self._read_all()

    async def cmd(self, cmd):
        if isinstance(cmd, List):
            for cmd_part in cmd:
                await self._execute(cmd_part)
        else:
            await self._execute(cmd)

    async def connect(self):
        self.process = await trio.lowlevel.open_process(
            [
                "ssh",
                "-T",
                "-i",
                self._local_key,
                f"{self.username}@{self.host}",
                "-o",
                "StrictHostKeyChecking=no"
            ],
            stdout=PIPE,
            stdin=PIPE,
            stderr=STDOUT,
        )

        await self.cmd([f"cd {self._working_directory}",
                        f"git checkout -f {self._working_branch}",
                        f"git pull",
                        'export PYTHONPATH="${PYTHONPATH}:' + f'{self._working_directory}/src"'
                        ])

        return self

    async def file_transfer(self, vm_path):
        ft_process = await trio.run_process(
            [
                "scp",
                "-o",
                "StrictHostKeyChecking=no",
                "-r",
                "-i",
                f"{self._local_key}",
                f"{self.username}@{self.host}:{self._working_directory}/{vm_path}/local",
                f"{vm_path}/vm",
            ]
        )
        self._logger.info(f"File transfer ended with exit code {ft_process.returncode}")
        return ft_process.returncode

    async def tunnel(self, server: "VM", port: int, key_file: str = None):
        key_path = self._remote_key or key_file
        if not key_path:
            raise Exception("No private key for ssh")
        await self.cmd(f"kill -9 $(lsof -t -i:{port})")
        await self.cmd(
            f"ssh -L {port}:127.0.0.1:{port} {server.username}@{server.host} -i {key_path} "
            f"-N -f -o StrictHostKeyChecking=no"
        )
        self._logger.info(f"Tunnel to host {server.host} opened on port {port}")

    async def exit(self):
        self.process.terminate()
