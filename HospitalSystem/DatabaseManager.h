#pragma once
#include <mysql/jdbc.h>
#include "Member.h";

class DatabaseManager
{
private:
	sql::Connection* conn;
	sql::Statement* stmt;
public:
	DatabaseManager();
	~DatabaseManager();
	
	bool Connect();
	bool ExecuteInsertQuery(const Member& member); //ȸ������
	bool ExcuteLoginQuery(const std::string id,   //�α���
		const std::string pw);
	bool CheckIdQuery(const std::string id);
};

