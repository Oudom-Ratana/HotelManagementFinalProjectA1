#ifndef EXCELUTIL_HPP
#define EXCELUTIL_HPP

#include <string>
#include <vector>
#include "Room.hpp"

void mergeBookingData(std::vector<Room>& roomLists, std::vector<Room>& bookedRooms);
void writeBookingToSheet(std::string filename, std::string sheetName, std::vector<Room>& roomLists);
std::vector<Room> readBookingFromSheet(std::string filename, std::string sheetName);
void writeVectorToExcel(std::string filename, std::vector<Room> roomLists);
std::vector<Room> readRoomFromExcel(std::string &filename);
void removeBookingFromSheet(std::string filename, std::string sheetName, int roomId);

#endif