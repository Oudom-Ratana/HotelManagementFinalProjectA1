#include "../include/ExcelUtil.hpp"
#include <xlnt/xlnt.hpp>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace xlnt;

void mergeBookingData(vector<Room>& roomLists, vector<Room>& bookedRooms)
{
    for (auto& booked : bookedRooms)
    {
        auto iD = find_if(roomLists.begin(), roomLists.end(),
                          [&](Room& bookData) { return bookData.getRoomId() == booked.getRoomId(); });
        if (iD != roomLists.end())
        {
            iD->name = booked.name;
            iD->gender = booked.gender;
            iD->phoneNum = booked.phoneNum;
            iD->checkInDate = booked.checkInDate;
            iD->checkOutDate = booked.checkOutDate;
            iD->setAvailable(0);
        }
    }
}

void writeBookingToSheet(string filename, string sheetName, vector<Room>& roomLists)
{
    workbook wb;
    try { wb.load(filename); }
    catch (...) { cout << "Cannot write into the file"; }

    worksheet ws;
    if (wb.contains(sheetName))
        ws = wb.sheet_by_title(sheetName);
    else
        ws = wb.create_sheet();
    ws.title(sheetName);

    ws.cell("A1").value("Guest Name");
    ws.cell("B1").value("Gender");
    ws.cell("C1").value("Phone Number");
    ws.cell("D1").value("Check In Date");
    ws.cell("E1").value("Check Out Date");
    ws.cell("F1").value("Room ID");
    ws.cell("G1").value("Room Type");
    ws.cell("H1").value("Equipments");
    ws.cell("I1").value("Price Per Night");
    ws.cell("J1").value("Availability");

    int row = 2;
    for (auto& room : roomLists)
    {
        if (room.getAvailability() == 0)
        {
            ws.cell("A" + to_string(row)).value(room.name);
            ws.cell("B" + to_string(row)).value(room.gender);
            ws.cell("C" + to_string(row)).value(to_string(room.phoneNum));
            ws.cell("D" + to_string(row)).value(room.checkInDate);
            ws.cell("E" + to_string(row)).value(room.checkOutDate);
            ws.cell("F" + to_string(row)).value(to_string(room.getRoomId()));
            ws.cell("G" + to_string(row)).value(room.getRoomType());
            ws.cell("H" + to_string(row)).value(room.getEquipments());
            ws.cell("I" + to_string(row)).value(to_string(room.getPricePerNight()));
            ws.cell("J" + to_string(row)).value("Booked");
            row++;
        }
    }
    wb.save(filename);
    cout << "Booking sheet saved." << endl;
}

vector<Room> readBookingFromSheet(string filename, string sheetName)
{
    vector<Room> roomLists;
    workbook wb;
    try { wb.load(filename); }
    catch (...)
    {
        cout << "Cannot read data from file!" << endl;
        return roomLists;
    }
    if (!wb.contains(sheetName))
    {
        cout << "Sheet does not exist!" << endl;
        return roomLists;
    }
    worksheet ws = wb.sheet_by_title(sheetName);
    for (auto row : ws.rows(false))
    {
        if (row[0].to_string() == "Guest Name") continue;
        string name = row[0].to_string();
        string gender = row[1].to_string();
        int phoneNum = stoi(row[2].to_string());
        string checkInDate = row[3].to_string();
        string checkOutDate = row[4].to_string();
        int roomId = stoi(row[5].to_string());
        string roomType = row[6].to_string();
        string equipments = row[7].to_string();
        float pricePerNight = stof(row[8].to_string());
        int isAvailable = 0;
        Room bookData(name, gender, phoneNum, checkInDate, checkOutDate, roomId, roomType, equipments, pricePerNight, isAvailable);
        roomLists.push_back(bookData);
    }
    return roomLists;
}

void writeVectorToExcel(string filename, vector<Room> roomLists)
{
    workbook wb;
    try { wb.load(filename); }
    catch (...) { }

    if (wb.contains("Room Data"))
        wb.remove_sheet(wb.sheet_by_title("Room Data"));

    worksheet ws = wb.create_sheet();
    ws.title("Room Data");

    ws.cell("A1").value("Room ID");
    ws.cell("B1").value("Room Type");
    ws.cell("C1").value("Equipments");
    ws.cell("D1").value("Price Per Night");
    ws.cell("E1").value("Availability");

    int row = 2;
    for (auto& room : roomLists)
    {
        ws.cell("A" + to_string(row)).value(to_string(room.getRoomId()));
        ws.cell("B" + to_string(row)).value(room.getRoomType());
        ws.cell("C" + to_string(row)).value(room.getEquipments());
        ws.cell("D" + to_string(row)).value(to_string(room.getPricePerNight()));
        ws.cell("E" + to_string(row)).value(room.getAvailabilityString());
        row++;
    }
    wb.save(filename);
    cout << "Successfully saved data to excel file!!!" << endl;
}

vector<Room> readRoomFromExcel(string &filename)
{
    vector<Room> roomLists;
    workbook wb;
    try { wb.load(filename); }
    catch (...)
    {
        cout << "Cannot read the data from file" << endl;
        return roomLists;
    }
    if (!wb.contains("Room Data"))
    {
        cout << "Sheet 'Room Data' not found." << endl;
        return roomLists;
    }
    worksheet ws = wb.sheet_by_title("Room Data");
    for (auto row : ws.rows(false))
    {
        if (row[0].to_string() == "Room ID") continue;
        int roomId = stoi(row[0].to_string());
        string roomType = row[1].to_string();
        string equipments = row[2].to_string();
        float pricePerNight = stof(row[3].to_string());
        int isAvailable = (row[4].to_string() == "Booked") ? 0 : 1;
        roomLists.push_back(Room(roomId, roomType, equipments, pricePerNight, isAvailable));
    }
    return roomLists;
}

void removeBookingFromSheet(string filename, string sheetName, int roomId)
{
    workbook wb;
    try { wb.load(filename); }
    catch (...) { return; }

    if (!wb.contains(sheetName)) return;
    worksheet ws = wb.sheet_by_title(sheetName);

    int rowToDelete = -1;
    int row = 2;
    for (auto rowIter : ws.rows(false))
    {
        if (rowIter[0].to_string() == "Guest Name") continue;
        int currentRoomId = stoi(rowIter[5].to_string());
        if (currentRoomId == roomId)
        {
            rowToDelete = row;
            break;
        }
        row++;
    }
    if (rowToDelete != -1)
    {
        ws.delete_rows(rowToDelete, 1);
        wb.save(filename);
        cout << "Booking record removed from sheet." << endl;
    }
}