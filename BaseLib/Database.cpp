#include "Database.h"
#include "Def.h"
#include "Logger.h"
#include <cstdio>

Database::Database()
{
	try
	{
		driver = get_driver_instance();
		driver->threadInit();
	} catch (SQLException &e) {
		Logger::Log("# ERROR #\n");
		std::cout << "\t\tSQLException in "<< __FILE__ << " ("<< __FUNCTION__ <<") on line "<< __LINE__ << std::endl;
		std::cout << "\t\t" << e.what() << " (MySQL error code: " << e.getErrorCode() << " SQLState: " << e.getSQLState() << std::endl;
	}
}

Database::~Database()
{
	try
	{
		driver->threadEnd();
		delete m_conn;
		delete stmt;
		delete res;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR #\n");
		std::cout << "\t\tSQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "\t\t" << e.what() << " (MySQL error code: " << e.getErrorCode() << " SQLState: " << e.getSQLState() << std::endl;
	}
}

bool Database::Connect(char* host, char* database, char* user, char* password, int port)
{
	try
	{
		char _host[MAX_PATH];
		sprintf_s(_host, MAX_PATH, "tcp://%s:%d", host, port);
		m_conn = driver->connect(_host, user, password);
		if (m_conn->isValid())
		{
			m_conn->setSchema(database);
			stmt = m_conn->createStatement();
			Logger::Log("Connected to Database Server (%s:%d) [%s]\n", host, port, database);
			return true;
		}
		return false;
	}
	catch (SQLException &e) {
		Logger::Log("Cannot connect to MySQL Server\n");
		Logger::Log("# ERROR #\n");
		std::cout << "\t\tSQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "\t\t" << e.what() << " (MySQL error code: " << e.getErrorCode() << " SQLState: " << e.getSQLState() << std::endl;
	}
	return false;
}

bool Database::ChangeDB(char* db)
{
	try {
		stmt->close();
		m_conn->setSchema(db);
		stmt = m_conn->createStatement();
		return true;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR #\n");
		std::cout << "\t\tSQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "\t\t" << e.what() << " (MySQL error code: " << e.getErrorCode() << " SQLState: " << e.getSQLState() << std::endl;
	}
	return false;
}

bool Database::ExecuteQuery(char* Format, ...)
{
	char szQuery[6000];
	va_list ap;
	va_start(ap, Format);
	vsprintf_s(szQuery, Format, ap);
	va_end(ap);
	SQLString query = SQLString(szQuery);

	try {
		stmt->close();
		stmt = m_conn->createStatement();
		stmt->executeUpdate(query);
		return true;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR #\n");
		std::cout << "\t\tSQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "\t\t" << e.what() << " (MySQL error code: " << e.getErrorCode() << " SQLState: " << e.getSQLState() << std::endl;
	}
	return false;
}

bool Database::ExecuteSelect(char* Format, ...)
{
	char szQuery[6000];
	va_list ap;
	va_start(ap, Format);
	vsprintf_s(szQuery, Format, ap);
	va_end(ap);
	SQLString query = SQLString(szQuery);

	try {
		stmt->close();
		stmt = m_conn->createStatement();
		res = stmt->executeQuery(query);
		return true;
	}
	catch (SQLException &e) {
		Logger::Log("# ERROR #\n");
		std::cout << "\t\tSQLException in " << __FILE__ << " (" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cout << "\t\t" << e.what() << " (MySQL error code: " << e.getErrorCode() << " SQLState: " << e.getSQLState() << std::endl;
	}
	return false;
}

bool Database::Fetch()
{
	return res->next();
}

long double Database::getDouble(const char* index)
{
	return res->getDouble(index);
}

bool Database::getBoolean(const char* index)
{
	return res->getBoolean(index);
}

int Database::getInt(const char* index)
{
	return res->getInt(index);
}

std::string Database::getString(const char* index)
{
	return res->getString(index);
}

size_t Database::rowsCount()
{
	return res->rowsCount();
}