#ifndef FRAL_ENGINE_SQLITE_H
#define FRAL_ENGINE_SQLITE_H

#include "sqlite3.h"
#include <cstdlib>
#include <cstring>

const char *CREATE = "CREATE TABLE IF NOT EXISTS LOG (id INTEGER PRIMARY KEY AUTOINCREMENT, blob_val BLOB);";
const char *INSERT = "INSERT INTO LOG (blob_val) VALUES (?);";
const char *SELECT = "SELECT blob_val FROM LOG WHERE id = ?;";

class FRAL_SQLITE {
    public:
        FRAL_SQLITE(const char* fileName, bool create = false){
            sqlite3_open(fileName, &db);
            if(create){
                sqlite3_exec(db, CREATE, nullptr, nullptr, nullptr);
            }

            sql_prepare(INSERT, ins_stmt);
            sql_prepare(SELECT, sel_stmt);

        }

        virtual ~FRAL_SQLITE() = default;


        void sql_prepare(const char * core, sqlite3_stmt *&stmt){
            if(sqlite3_prepare_v2(db, core, -1, &stmt, nullptr) != SQLITE_OK){
                std::cerr << "Prepare error: " << sqlite3_errmsg(db) << std::endl;
                exit(1);
            }
        }

        void sql_bind_blob(void * blob, int sz){
            if(sqlite3_bind_blob(ins_stmt, 1, blob, sz, SQLITE_STATIC) != SQLITE_OK){
                std::cerr << "Insert bind error!" << std::endl;
                exit(1);
            }
        }

        void sql_bind_int(int int_val){
            if(sqlite3_bind_int(sel_stmt, 1, int_val) != SQLITE_OK){
                std::cerr << "Insert bind error!" << std::endl;
                exit(1);
            }
        }

        int execute(sqlite3_stmt * stmt){

            for(;;){

                auto status = sqlite3_step(stmt);

                switch (status) {

                    case SQLITE_ROW:
                    case SQLITE_DONE:
                        return status;

                    case SQLITE_LOCKED:
                    case SQLITE_BUSY:
                        continue;

                    default:
                        std::cerr << "Execute error: " << sqlite3_errmsg(db) << std::endl;
                        exit(1);
                }
            }
        }

        void append(void* blob, int sz){

            sql_bind_blob(blob, sz);
            execute(ins_stmt);
            sqlite3_reset(ins_stmt);

        }

        void *load(int idx) {

            sql_bind_int(idx);
            auto status = execute(sel_stmt);

            if (status == SQLITE_ROW) {
                int blobSize = sqlite3_column_bytes(sel_stmt, 0);
                auto blob = malloc(blobSize);
                memcpy(blob, sqlite3_column_blob(sel_stmt, 0), blobSize);
                sqlite3_reset(sel_stmt);
                return blob;
            }

            sqlite3_reset(sel_stmt);

            return nullptr;
        }

    protected:

        sqlite3 *db;
        sqlite3_stmt *ins_stmt;
        sqlite3_stmt *sel_stmt;

    };


#endif //FRAL_ENGINE_SQLITE_H
