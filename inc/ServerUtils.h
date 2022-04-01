/*
 * ServerUtils.h
 *
 *  Created on: 31 de mar. de 2022
 *      Author: jonathas
 */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>
#include <ctime>

class ServerUtils
{
/***Attributes***/
private:

public:
    int         conPort;
    int         maxFileSize;		// Size given in bytes
    double      conTimeout;			// Time given in seconds
    std::string filePrefix;

/***Member Functions***/
private:

public:
    ServerUtils();

    // Setters
    void setConPort(int conPort);
    void setMaxFileSize(int maxFileSize) { this->maxFileSize = maxFileSize; }
    void setFilePrefix(std::string filePrefix) { this->filePrefix = filePrefix; }
    void setConTimeout(double conTimeout) { this->conTimeout = conTimeout; }

    // Getters
    int getConPort() { return this->conPort; }
    int getMaxFileSize() { return this->maxFileSize; }
    std::string getFilePrefix() { return this->filePrefix; }
    double getConTimeout() { return this->conTimeout; }

    // Others
    void processConfFile(std::ifstream& confFile);
    void saveReceivedData(std::string filePrefix, std::string receivedData);
    std::string getTimeStamp();
};


// Simple logger class
class Logger
{
/***Attributes***/
private:
		int logLevel;
public:
		const int logLevelInfo  = 2;
		const int logLevelWarn  = 1;
		const int logLevelError = 0;

/***Methods***/
private:

public:
		Logger();

		// Setter
		void setLevel(int Level) { this->logLevel = Level; }

		// Getter
		const char* getLevel();

		// Member Functions
		void error(const char* message);
		void warn (const char* message);
		void info (const char* message);
};
