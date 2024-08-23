/**                 数据库
*/
// --------------------------------------------
// 读取poly
// --------------------------------------------
void read_poly_table(const std::string& databaseFile, const std::string& tableName, EPOLYS& polys)
{
    // Open the database
    sqlite3* db;
    int rc = sqlite3_open(databaseFile.c_str(), &db);
    if (rc != SQLITE_OK) {
        // Handle error
        MSGBOX("No " << databaseFile);
        return;
    }

    // Prepare SELECT statement
    sqlite3_stmt* stmt;
    std::string selectQuery = "SELECT polyid, x, y, z, type FROM " + tableName + ";";
    rc = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_close(db);
        return;
    }

    // Execute statement
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int polyid = sqlite3_column_int(stmt, 0);
        double x = sqlite3_column_double(stmt, 1);
        double y = sqlite3_column_double(stmt, 2);
        double z = sqlite3_column_double(stmt, 3);
        int type = sqlite3_column_int(stmt, 4);

        // Add point to appropriate poly
        if (polyid >= polys.size()) {
            polys.resize(polyid + 1);
        }
        polys[polyid].emplace_back(vec3(x, y, z), -1, type);
    }

    // Finalize statement
    sqlite3_finalize(stmt);

    // Close database connection
    sqlite3_close(db);
}
// --------------------------------------------
// 保存 poly
// --------------------------------------------
void save_poly_table(const std::string& databaseFile, const std::string& tableName, const EPOLYS& polys)
{
    PRINT("save_poly_table: " << polys.size());

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open(databaseFile.c_str(), &db);
    if (rc != SQLITE_OK) {
        // Handle error
        MSGBOX("No " << databaseFile);
        return;
    }

    // Drop table if it exists
    std::string dropTableQuery = "DROP TABLE IF EXISTS " + tableName + ";";
    rc = sqlite3_exec(db, dropTableQuery.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_close(db);
        return;
    }

    // Create table
    std::string createTableQuery = "CREATE TABLE " + tableName + " (polyid INTEGER, x REAL, y REAL, z REAL, type INTEGER);";
    rc = sqlite3_exec(db, createTableQuery.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_close(db);
        return;
    }

    // Begin transaction
    rc = sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_close(db);
        return;
    }

    // Prepare INSERT statement
    sqlite3_stmt* stmt;
    std::string insertQuery = "INSERT INTO " + tableName + " (polyid, x, y, z, type) VALUES (?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insertQuery.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_close(db);
        return;
    }

    // Execute statement for each poly
    for (int i = 0; i < polys.size(); i++) {
        for (const auto& point : polys[i]) {
            sqlite3_bind_int(stmt, 1, i);
            sqlite3_bind_double(stmt, 2, point.p.x);
            sqlite3_bind_double(stmt, 3, point.p.y);
            sqlite3_bind_double(stmt, 4, point.p.z);
            sqlite3_bind_int(stmt, 5, 0);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                // Handle error
                sqlite3_finalize(stmt);
                sqlite3_exec(db, "ROLLBACK;", 0, 0, 0);
                sqlite3_close(db);
                return;
            }

            sqlite3_reset(stmt);
        }
    }

    // Finalize statement
    sqlite3_finalize(stmt);

    // Commit transaction
    rc = sqlite3_exec(db, "COMMIT;", 0, 0, 0);
    if (rc != SQLITE_OK) {
        // Handle error
        sqlite3_close(db);
        return;
    }

    // Close database connection
    sqlite3_close(db);
}
// --------------------------------------------
// 读取submesh
// --------------------------------------------
void read_sm_table(const std::string& databaseFile, std::map<int, submesh>& sm_map)
{
    // Open the database
    sqlite3* db;
    int rc = sqlite3_open(databaseFile.c_str(), &db);
    if (rc != SQLITE_OK) {
        // Handle error
        MSGBOX("No " << databaseFile);
        return;
    }
    {
        // Prepare SELECT statement
        sqlite3_stmt* stmt;
        std::string selectQuery = "SELECT smid, x, y, z, nx, ny, nz FROM vertices;";
        rc = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            // Handle error
            sqlite3_close(db);
            return;
        }

        // Execute statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int smid = sqlite3_column_int(stmt, 0);
            vec3 p, n;
            p.x = sqlite3_column_double(stmt, 1);
            p.y = sqlite3_column_double(stmt, 2);
            p.z = sqlite3_column_double(stmt, 3);
            n.x = sqlite3_column_double(stmt, 4);
            n.y = sqlite3_column_double(stmt, 5);
            n.z = sqlite3_column_double(stmt, 6);

            sm_map[smid].vertices.emplace_back(p, n);
        }

        // Finalize statement
        sqlite3_finalize(stmt);
    }
    {
        // Prepare SELECT statement
        sqlite3_stmt* stmt;
        std::string selectQuery = "SELECT smid, v1, v2, v3 FROM tris;";
        rc = sqlite3_prepare_v2(db, selectQuery.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            // Handle error
            sqlite3_close(db);
            return;
        }

        // Execute statement
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int smid = sqlite3_column_int(stmt,  0);
            int v1 = sqlite3_column_double(stmt, 1);
            int v2 = sqlite3_column_double(stmt, 2);
            int v3 = sqlite3_column_double(stmt, 3);

            sm_map[smid].tris.emplace_back(v1, v2, v3);
        }

        // Finalize statement
        sqlite3_finalize(stmt);
    }
    // Close database connection
    sqlite3_close(db);
}