#include "ServerUtils.h"

ServerUtils::ServerUtils()
{
    this->conPort     = 60079;
    this->maxFileSize = 1024;
    this->conTimeout  = 2.0;
    this->filePrefix  = "COGNYTESVR";
}

void ServerUtils::setConPort(int conPort)
{
	// Validating given port number
	if (conPort > 1024)
	{
		this->conPort = conPort;
	}
	else
	{
		std::cout << "Invalid port number. Setting default!" << std::endl;
	}
}

void ServerUtils::processConfFile(std::ifstream& confFile)
{
	int tokenPos;
	std::string paramField;
	std::string paramValue;

	while(std::getline(confFile,paramField))
	{
		tokenPos = paramField.find("=");

		if (tokenPos < 0)
		{
			std::cout << "Invalid parameter." << std::endl;
		}
		else
		{
			paramValue = paramField.substr(tokenPos + 1,
					paramField.length() - 1);
			paramField = paramField.substr(0, tokenPos);

			if (!paramField.compare("PORTNUMBER"))
			{
				this->setConPort(std::stoi(paramValue));
			}
			else if (!paramField.compare("FILE_SIZE"))
			{
				this->setMaxFileSize(std::stoi(paramValue));
			}
			else if (!paramField.compare("FILE_PREFIX"))
			{
				this->setFilePrefix(paramValue);
			}
			else if (!paramField.compare("TIMEOUT"))
			{
				this->setConTimeout(std::stod(paramValue));
			}
			else
			{
				std::cout << "Invalid parameter." << std::endl;
			}
		}
		std::cout << "\t" << paramField << ": " << paramValue << std::endl;
	}
}

void ServerUtils::saveReceivedData(std::string filePrefix,
		std::string receivedData)
{
	std::fstream destinationFile;

	// Informs how much room is left on the destination file
	unsigned int storageLeft = this->getMaxFileSize();

	// Attaching current timestamp to fileName
	std::string fileName = filePrefix + this->getTimeStamp();

	// Opening file in append mode
	destinationFile.open(fileName, std::ios::out | std::ios::app);
	std::cout << "Opening file for saving" << std::endl;

	// Check if opening went well
	if (!destinationFile)
	{
		std::cout << "Error while saving" << std::endl;
	}
	else
	{
		// How much data is left to be saved
		int remainingData = receivedData.length();

		std::string::iterator it = receivedData.begin();

		// Append new savings while there is data or room left
		int indexSave = 0;
		do
		{
			// Verifies if the save is to be performed for more than one file.
			// If so, closes the current file stream, and opens a new one with
			// its name updated according to the number of files needed for the
			// save operation.
			if (indexSave > 0)
			{
				// Resets storage information
				storageLeft = this->getMaxFileSize();

				// Closing current save file
				destinationFile.close();

				// Opening a new file in append mode with updated name
				destinationFile.open((fileName + "_" + std::to_string(indexSave)),
						std::ios::out | std::ios::app);

				// Check if opening went well
				if (!destinationFile)
				{
					std::cout << "Error while saving" << std::endl;
					return;
				}
			}

			// Saving to file one character per time
			for (; (it != receivedData.end() && storageLeft > 0); ++it)
			{
				destinationFile << *it;
				remainingData--;
				storageLeft--;
			}

			// Counting index for incremental file saving
			indexSave++;

		} while (remainingData > 0);
		std::cout << "Saving occurred successfully!" << std::endl;

		// Clearing buffer
		receivedData.clear();
	}

	// Closing file after save
	destinationFile.close();
}

// Returns current timestamp in the format YYYYMMddHHmmss
std::string ServerUtils::getTimeStamp()
{
	time_t rawTime;
	struct tm * timeInfo;
	std::string timeStamp = "_";

	time (&rawTime);
	timeInfo = localtime(&rawTime);

	timeStamp += std::to_string(timeInfo->tm_year + 1900);
	timeStamp += std::to_string(timeInfo->tm_mon + 1);		// Note that tm struct
	timeStamp += std::to_string(timeInfo->tm_mday);			// counts months from
	timeStamp += std::to_string(timeInfo->tm_hour);			// 0 to 11 (Jan-Dez).
	timeStamp += std::to_string(timeInfo->tm_min);
	timeStamp += std::to_string(timeInfo->tm_sec);

	return timeStamp;
}

Logger::Logger()
{
	logLevel = logLevelInfo;
}

const char* Logger::getLevel()
{
	switch (this->logLevel)
	{
	case 0:
		return "Error";
		break;
	case 1:
		return "Warning";
		break;
	default:
		return "Info";
		break;
	}
}

void Logger::error(const char* message)
{
	if (this->logLevel >= this->logLevelError)
		std::cout << "[ERROR]: " << message << std::endl;
}

void Logger::warn(const char* message)
{
	if (this->logLevel >= this->logLevelWarn)
		std::cout << "[WARNING]: " << message << std::endl;
}

void Logger::info (const char* message)
{
	if (this->logLevel >= this->logLevelInfo)
		std::cout << "[INFO]: " << message << std::endl;
}
