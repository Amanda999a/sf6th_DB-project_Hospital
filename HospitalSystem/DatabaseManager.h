#pragma once
#include <mysql/jdbc.h>
#include "Doctor.h";
#include <nlohmann/json.hpp> 
#include <fstream> 


class DatabaseManager
{
private:
	sql::Connection* conn;
	sql::Statement* stmt;
public:
	DatabaseManager();
	~DatabaseManager();
	
	bool Connect();
	bool ExecuteInsertQuery(const Doctor& member); //ȸ������
	bool ExcuteLoginQuery(const std::string id,   //�α���
		const std::string pw);
	bool CheckIdQuery(const std::string id);
};

