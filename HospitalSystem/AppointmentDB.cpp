#include "pch.h"
#include "AppointmentDB.h"
/*
	AppointmentDB();
	~AppointmentDB();
	std::vector<Appointment> todayAppointment(std::string doctorId);

*/

AppointmentDB::AppointmentDB() {
	this->con = nullptr;
}
AppointmentDB::~AppointmentDB() {
	delete this->con;
}
bool AppointmentDB::Connect() {
	try {
		sql::mysql::MySQL_Driver* driver = sql::mysql::get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "root");
		con->setSchema("Hospital");

		// db �ѱ� ������ ���� ���� 
		stmt = con->createStatement();
		stmt->execute("set names euckr");
		if (stmt) { delete stmt; stmt = nullptr; }
		return true;
	}
	catch (sql::SQLException e) {
		std::cout << e.what() << std::endl;
		return false;
	}
}
std::vector<AppointmentDto> AppointmentDB::todayAppointment(std::string doctorId) {
	sql::PreparedStatement *pstmt = nullptr;
	std::vector<AppointmentDto> appointments;
	try {
		std::string query = "SELECT patient_phone_number, appointment_date, symptoms FROM appointment WHERE doctor_id = ? AND DATE(appointment_date) = CURDATE()";
		pstmt = con->prepareStatement(query);
		pstmt->setString(1, doctorId);
		sql::ResultSet* res = pstmt->executeQuery();
		while (res->next()) {
			/*(std::string doctorid, std::string patientid,
		std::string appointDate,std::string symptons);
			*/
			AppointmentDto app(doctorId, res->getString("patient_phone_number"),
				res->getString("appointment_date"), res->getString("symptoms"));
			appointments.push_back(app);
		}
		return appointments;
	}
	catch (sql::SQLException e) {
		std::cout << e.what() << std::endl;
		delete pstmt;
	}



}



