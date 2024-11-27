#include "pch.h"
#include "DatabaseManager.h"
#include <nlohmann/json.hpp> // nlohmann-json ��� �߰�
#include <fstream> // ���� �����

using json = nlohmann::json;

DatabaseManager::DatabaseManager() {
    this->conn = nullptr;
}

DatabaseManager::~DatabaseManager() {
    delete conn;
}

bool DatabaseManager::Connect() {
    try {
        // JSON ���� �б� (config.json)
        std::ifstream inputFile("config.json");
        if (!inputFile.is_open()) {
            std::cerr << "������ �� �� �����ϴ�!" << std::endl;
            return false;
        }

        // JSON ������ �Ľ�
        json config;
        inputFile >> config;

        // JSON���� �� ����
        std::string host = config["database"]["host"];
        std::string username = config["database"]["username"];
        std::string password = config["database"]["password"];
        std::string schema = config["database"]["schema"];
        std::string charset = config["database"]["charset"];

        // MySQL ����
        sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
        conn = driver->connect(host, username, password);
        conn->setSchema(schema);

        // DB �ѱ� ������ ���� ����
        stmt = conn->createStatement();
        stmt->execute("set names " + charset);  // charset ����
        if (stmt) { delete stmt; stmt = nullptr; }

        return true;
    }
    catch (sql::SQLException& e) {
        std::cout << "SQL ����: " << e.what() << std::endl;
        return false;
    }
    catch (const std::exception& e) {
        std::cout << "�Ϲ� ����: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::ExecuteInsertQuery(const Doctor& member) {
    sql::PreparedStatement* pstmt = nullptr;
    try {
        std::string query = "insert into doctor (id,password,name,phone_number,employee_number) values(?,?,?,?,?)";
        pstmt = this->conn->prepareStatement(query);

        pstmt->setString(1, member.getId());
        pstmt->setString(2, member.getPassword());
        pstmt->setString(3, member.getName());
        pstmt->setString(4, member.getPhoneNumber());
        pstmt->setString(5, member.getEmployeeNumber());
        pstmt->executeUpdate();
        delete pstmt;
        return true;
    }
    catch (sql::SQLException e) {
        std::cout << e.what() << std::endl;
        delete pstmt;
        return false;
    }
}

bool DatabaseManager::ExcuteLoginQuery(const std::string id, const std::string pw) {
    sql::PreparedStatement* pstmt = nullptr;
    try {
        std::string query = "select * from doctor where id = ? and password =?";
        pstmt = this->conn->prepareStatement(query);
        pstmt->setString(1, id);
        pstmt->setString(2, pw);
        sql::ResultSet* res = pstmt->executeQuery();
        int count = 0;
        while (res->next()) {
            count++;
        }
        if (count == 1) {
            delete pstmt;
            delete res;
            return true;
        }
        delete pstmt;
        delete res;
        return false;
    }
    catch (sql::SQLException e) {
        std::cout << e.what() << std::endl;
        delete pstmt;
        return false;
    }
}

bool DatabaseManager::CheckIdQuery(const std::string id) {
    std::string query = "select * from doctor where id =?";
    sql::PreparedStatement* pstmt = nullptr;
    try {
        pstmt = conn->prepareStatement(query);
        pstmt->setString(1, id);
        sql::ResultSet* res = pstmt->executeQuery();
        while (res->next()) {
            delete pstmt;
            delete res;
            return false;
        }
        delete pstmt;
        delete res;
        return true;
    }
    catch (sql::SQLException e) {
        std::cout << e.what() << std::endl;
        delete pstmt;
        return false;
    }
}
