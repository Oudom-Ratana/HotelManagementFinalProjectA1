#include <iostream>
#include <iomanip>
#include <vector>
#include <tabulate/table.hpp>
#include <xlnt/xlnt.hpp>
using namespace std;
using namespace tabulate;
using namespace xlnt;

class Room
{
private:
    int roomId = 0;
    string roomType;
    string equipments;
    float pricePerNight = 0;
    int isAvailable = 1;

public:
    string name;
    string gender;
    int phoneNum;
    int roomIdBook;
    string checkInDate;
    string checkOutDate;
    char comfirmBooking;

public:
    Room() {}
    Room(int roomId, string roomtype, string equipments, float pricePerNight, int isAvailable)
    {
        this->roomId = roomId;
        this->roomType = roomtype;
        this->equipments = equipments;
        this->pricePerNight = pricePerNight;
        this->isAvailable = isAvailable;
    }
    Room(string name, string gender, int phoneNum, string checkInDate, string checkOutDate, int roomId, string roomtype, string equipments, float pricePerNight, int isAvailable)
    {
        this->name = name;
        this->gender = gender;
        this->phoneNum = phoneNum;
        this->checkInDate = checkInDate;
        this->checkOutDate = checkOutDate;
        this->roomId = roomId;
        this->roomType = roomtype;
        this->equipments = equipments;
        this->pricePerNight = pricePerNight;
        this->isAvailable = isAvailable;
    }
    void input()
    {
        cout << "Enter roomID: ";
        cin >> roomId;
        cin.ignore();
        cout << "Enter Room Type(Single, Double, VIPsingle, VIPdouble): ";
        cin >> roomType;
        cin.ignore();
        cout << "Enter Equipments: ";
        getline(cin, equipments);
        cout << "Enter price/Night: ";
        cin >> pricePerNight;
    }
    void update()
    {
        cin.ignore();
        cout << "Enter Room Type(Single, Double, VIPsingle, VIPdouble): ";
        cin >> roomType;
        cout << "Enter Equipments: ";
        cin.ignore();
        getline(cin, equipments);
        cout << "Enter price/Night: ";
        cin >> pricePerNight;
    }
    string getName() { return name; }
    string getGender() { return gender; }
    int getPhoneNum() { return phoneNum; }
    string getCheckInDate() { return checkInDate; }
    string getCheckOutDate() { return checkOutDate; }
    int getRoomId() { return roomId; }
    string getRoomType() { return roomType; }
    string getEquipments() { return equipments; }
    float getPricePerNight() { return pricePerNight; }
    int getAvailability() { return isAvailable; }
    void setRoomId(int roomId) { this->roomId = roomId; }
    void setRoomType(string roomType) { this->roomType = roomType; }
    void setEquipment(string equipments) { this->equipments = equipments; }
    void setPricePerNight(float pricePerNight) { this->pricePerNight = pricePerNight; }
    void setAvailable(int isAvailable) { this->isAvailable = isAvailable; }
    string getAvailabilityString()
    {
        if (isAvailable == 1)
            return "Available";
        if (isAvailable == 0)
            return "Booked";
        return "Unknown";
    }
};

void mergeBookingData(vector<Room> &roomLists, vector<Room> &bookedRooms)
{
    for (auto &booked : bookedRooms)
    {
        auto iD = find_if(roomLists.begin(), roomLists.end(),
                          [&](Room &bookData)
                          { return bookData.getRoomId() == booked.getRoomId(); });
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

void writeBookingToSheet(string filename, string sheetName, vector<Room> &roomLists)
{
    workbook wb;
    try
    {
        wb.load(filename);
    }
    catch (...)
    {
        cout << "Cannot write into the file";
    }

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
    for (auto &room : roomLists)
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
    try
    {
        wb.load(filename);
    }
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
        if (row[0].to_string() == "Guest Name")
            continue;
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
    try
    {
        wb.load(filename);
    }
    catch (...)
    {
    }

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
    for (auto &room : roomLists)
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
    try
    {
        wb.load(filename);
    }
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
        if (row[0].to_string() == "Room ID")
            continue;
        int roomId = stoi(row[0].to_string());
        string roomType = row[1].to_string();
        string equipments = row[2].to_string();
        float pricePerNight = stof(row[3].to_string());
        int isAvailable = (row[4].to_string() == "Booked") ? 0 : 1;
        roomLists.push_back(Room(roomId, roomType, equipments, pricePerNight, isAvailable));
    }
    return roomLists;
}

class Guest : public Room
{
public:
    void bookingRoom(vector<Room> &roomLists, string &filename)
    {
        cin.ignore();
        cout << "Enter Your Name: ";
        getline(cin, name);
        cout << "Enter Your gender: ";
        getline(cin, gender);
        cout << "Enter Your Phone Number: ";
        cin >> phoneNum;
        cout << "Enter Room ID: ";
        cin >> roomIdBook;
        cin.ignore();
        cout << "Enter CheckIn Date(dd/mm/yyyy): ";
        getline(cin, checkInDate);
        cout << "Enter CheckOut Date(dd/mm/yyyy): ";
        getline(cin, checkOutDate);
        cout << "Comfirm Booking?(Y/N): ";
        cin >> comfirmBooking;
        if (comfirmBooking == 'y' || comfirmBooking == 'Y')
        {
            auto result = find_if(roomLists.begin(), roomLists.end(),
                                  [&](Room &room)
                                  { return room.getRoomId() == roomIdBook && room.getAvailability() == 1; });
            if (result != roomLists.end())
            {
                result->setAvailable(0);
                result->name = name;
                result->gender = gender;
                result->phoneNum = phoneNum;
                result->checkInDate = checkInDate;
                result->checkOutDate = checkOutDate;
                writeVectorToExcel(filename, roomLists);
                writeBookingToSheet(filename, "Booked Room", roomLists);
                cout << "Successfully Booked the room!" << endl;
            }
            else
            {
                cout << "Room Not found!" << endl;
            }
        }
        else
        {
            cout << "Booking Cancelled!" << endl;
        }
    }

    void showReceipt(vector<Room> &roomLists)
    {
        auto result = find_if(roomLists.begin(), roomLists.end(),
                              [&](Room &room)
                              { return room.getAvailability() == 0; });
        if (result != roomLists.end())
        {
            int guestPhone;
            cout << "Enter your phone number to see your receipt: ";
            cin >> guestPhone;
            bool found = false;
            Table table;
            table.add_row({"Name", "Gender", "Phone Number", "CheckIn Date", "RoomID", "RoomType", "Equipments", "Price/night($)"});
            for (auto &room : roomLists)
            {
                if ((room.getAvailability() == 0) && (room.phoneNum == guestPhone))
                {
                    stringstream ss;
                    ss << fixed << setprecision(2) << room.getPricePerNight();
                    string priceStr = ss.str();
                    table.add_row({room.name, room.gender, to_string(room.phoneNum), room.checkInDate,
                                   to_string(room.getRoomId()), room.getRoomType(), room.getEquipments(), priceStr});
                    found = true;
                }
            }
            if (found == true)
            {
                table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
                cout << table << endl;
            }
            else
            {
                cout << "No booking found for phone number " << guestPhone << endl;
            }
        }
        else
        {
            cout << "No Room Booked !!!" << endl;
        }
    }

    void searchGuestRoomById(vector<Room> &roomLists)
    {
        int roomId;
        cout << "Enter Room ID: ";
        cin >> roomId;
        auto result = find_if(roomLists.begin(), roomLists.end(),
                              [&](Room &room)
                              { return room.getRoomId() == roomId && room.getAvailability() == 1; });
        if (result != roomLists.end())
        {
            cout << "Room  <" << roomId << "> Found!" << endl;
            Table table;
            table.add_row({"RoomID", "RoomType", "Equipments", "Price/night($)", "Availability"});
            stringstream ss;
            Room room = *result;
            ss << fixed << setprecision(2) << room.getPricePerNight();
            string priceStr = ss.str();
            table.add_row({to_string(room.getRoomId()), room.getRoomType(), room.getEquipments(), priceStr, room.getAvailabilityString()});
            table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
            cout << table << endl;
        }
        else
        {
            cout << "Room Not Found!" << endl;
        }
    }

    void displayRoomForGuest(vector<Room> &roomlists)
    {
        Table table;
        table.add_row({"RoomID", "RoomType", "Equipments", "Price/night($)", "Availability"});
        for (auto rooms : roomlists)
        {
            stringstream ss;
            ss << fixed << setprecision(2) << rooms.getPricePerNight();
            string priceStr = ss.str();
            if (rooms.getAvailabilityString() == "Available")
                table.add_row({to_string(rooms.getRoomId()), rooms.getRoomType(), rooms.getEquipments(), priceStr, rooms.getAvailabilityString()});
        }
        table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
        cout << table << endl;
    }

    void sortRoomByIdAsc(vector<Room> roomLists)
    {
        sort(roomLists.begin(), roomLists.end(),
             [&](Room &room1, Room &room2)
             { return room1.getRoomId() < room2.getRoomId(); });
        displayRoomForGuest(roomLists);
    }

    void sortRoomByIdDsc(vector<Room> roomLists)
    {
        sort(roomLists.begin(), roomLists.end(),
             [&](Room &room1, Room &room2)
             { return room1.getRoomId() > room2.getRoomId(); });
        displayRoomForGuest(roomLists);
    }

    void sortRoomByPriceAsc(vector<Room> roomLists)
    {
        sort(roomLists.begin(), roomLists.end(),
             [&](Room &room1, Room &room2)
             { return room1.getPricePerNight() < room2.getPricePerNight(); });
        displayRoomForGuest(roomLists);
    }

    void sortRoomByPriceDsc(vector<Room> roomLists)
    {
        sort(roomLists.begin(), roomLists.end(),
             [&](Room &room1, Room &room2)
             { return room1.getPricePerNight() > room2.getPricePerNight(); });
        displayRoomForGuest(roomLists);
    }
};

class Admin
{
public:
    void displayRoom(vector<Room> &roomLists)
    {
        Table table;
        table.add_row({"RoomID", "RoomType", "Equipments", "Price/night($)", "Availability"});
        for (auto rooms : roomLists)
        {
            stringstream ss;
            ss << fixed << setprecision(2) << rooms.getPricePerNight();
            string priceStr = ss.str();
            table.add_row({to_string(rooms.getRoomId()), rooms.getRoomType(), rooms.getEquipments(), priceStr, rooms.getAvailabilityString()});
        }
        table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
        cout << table << endl;
    }

    void searchRoomById(vector<Room> &roomLists)
    {
        int roomId;
        cout << "Enter Room ID: ";
        cin >> roomId;
        auto result = find_if(roomLists.begin(), roomLists.end(),
                              [&](Room &room)
                              { return room.getRoomId() == roomId; });
        if (result != roomLists.end())
        {
            cout << "Room  <" << roomId << "> Found!" << endl;
            Table table;
            table.add_row({"RoomID", "RoomType", "Equipments", "Price/night($)", "Availability"});
            stringstream ss;
            Room room = *result;
            ss << fixed << setprecision(2) << room.getPricePerNight();
            string priceStr = ss.str();
            table.add_row({to_string(room.getRoomId()), room.getRoomType(), room.getEquipments(), priceStr, room.getAvailabilityString()});
            table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
            cout << table << endl;
        }
        else
        {
            cout << "Room Not Found!" << endl;
        }
    }

    void sortRoomByIdAsc(vector<Room> roomLists)
    {
        sort(roomLists.begin(), roomLists.end(),
             [&](Room &room1, Room &room2)
             { return room1.getRoomId() < room2.getRoomId(); });
        displayRoom(roomLists);
    }

    void sortRoomByIdDsc(vector<Room> roomLists)
    {
        sort(roomLists.begin(), roomLists.end(),
             [&](Room &room1, Room &room2)
             { return room1.getRoomId() > room2.getRoomId(); });
        displayRoom(roomLists);
    }

    void sortRoomByPriceAsc(vector<Room> roomLists)
    {
        sort(roomLists.begin(), roomLists.end(),
             [&](Room &room1, Room &room2)
             { return room1.getPricePerNight() < room2.getPricePerNight(); });
        displayRoom(roomLists);
    }

    void sortRoomByPriceDsc(vector<Room> roomLists)
    {
        sort(roomLists.begin(), roomLists.end(),
             [&](Room &room1, Room &room2)
             { return room1.getPricePerNight() > room2.getPricePerNight(); });
        displayRoom(roomLists);
    }
};

void printMenuAsTable(vector<string> menuList, string menuRole)
{
    Table table;
    table.add_row({menuRole});
    for (string list : menuList)
        table.add_row({list});
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void printLabel(string label)
{
    Table table;
    table.add_row({label});
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void pressEnter()
{
    cout << "---------<Press Enter to continue>---------" << endl;
    cin.ignore();
    cin.get();
}
void removeBookingFromSheet(string filename, string sheetName, int roomId)
{
    workbook wb;
    try
    {
        wb.load(filename);
    }
    catch (...)
    {
        return;
    }

    if (!wb.contains(sheetName))
        return;
    worksheet ws = wb.sheet_by_title(sheetName);

    int rowToDelete = -1;
    int row = 2;
    for (auto rowIter : ws.rows(false))
    {
        if (rowIter[0].to_string() == "Guest Name")
            continue;
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

int main()
{
    system("cls");
    vector<Room> roomLists;
    string filename = "roomListsStorage.xlsx";
    vector<Guest> bookHistory;

    roomLists = readRoomFromExcel(filename);
    if (roomLists.empty())
    {
        roomLists.push_back(Room(101, "Single", "Fan, Wifi, TV", 18, 1));
        roomLists.push_back(Room(102, "Single", "Fan, Wifi, TV", 18, 1));
        roomLists.push_back(Room(103, "Double", "AC, Wifi, TV", 28, 1));
        roomLists.push_back(Room(104, "VIPsingle", "AC, FastWifi, HDTV, foods, drinks", 50, 1));
        roomLists.push_back(Room(105, "VIPdouble", "AC, FastWifi, HDTV, foods, drinks, snacks", 100, 1));
        writeVectorToExcel(filename, roomLists);
    }

    vector<Room> bookedRooms = readBookingFromSheet(filename, "Booked Room");
    mergeBookingData(roomLists, bookedRooms);

    vector<string> adminMenuList = {
        "1. Add new room ",
        "2. Update Room for quest ",
        "3. Show All Rooms ",
        "4. Search Room ",
        "5. Delete Room ",
        "6. Check out Room ",
        "7. Logout "};
    vector<string> guestMenuList = {
        "1. Show All Rooms ",
        "2. Sort Rooms  ",
        "3. Booking Room ",
        "4. Search Room ",
        "5. Show Receipt ",
        "6. Logout "};

    string password, name;
    int countError = 0;
    do
    {
        system("cls");
        cout << "Enter Username: ";
        cin >> name;
        cout << "Enter Password: ";
        cin >> password;

        if (name == "admin" && password == "admin123")
        {
            int adminOption;
            do
            {
                system("cls");
                bookedRooms = readBookingFromSheet(filename, "Booked Room");
                roomLists = readRoomFromExcel(filename);
                mergeBookingData(roomLists, bookedRooms);
                printMenuAsTable(adminMenuList, "Admin Console App");
                cout << "CHoose your option(1-6): ";
                cin >> adminOption;

                switch (adminOption)
                {
                case 1:
                {
                    printLabel("Add new room");
                    Room newRoom;
                    newRoom.input();
                    roomLists.push_back(newRoom);
                    writeVectorToExcel(filename, roomLists);
                    cout << "New Room added successfully!" << endl;
                }
                break;
                case 2:
                {
                    int roomId;
                    printLabel("Update Room");
                    cout << "Enter Room ID: ";
                    cin >> roomId;
                    auto result = find_if(roomLists.begin(), roomLists.end(),
                                          [&](Room &room)
                                          { return room.getRoomId() == roomId; });
                    if (result != roomLists.end())
                    {
                        cout << "Room ID found!" << endl;
                        Room roomUpdate = *result;
                        roomUpdate.update();
                        *result = roomUpdate;
                        writeVectorToExcel(filename, roomLists);
                        cout << "Successfully Updated the room!" << endl;
                    }
                    else
                    {
                        cout << "Room Not found!" << endl;
                    }
                }
                break;
                case 3:
                {
                    printLabel("Show all Room");
                    Admin a1;
                    a1.displayRoom(roomLists);
                }
                break;
                case 4:
                {
                    printLabel("Search Room By ID");
                    Admin admin;
                    admin.searchRoomById(roomLists);
                }
                break;
                case 5:
                {
                    int roomId;
                    cout << "Enter Room ID: ";
                    cin >> roomId;
                    auto result = find_if(roomLists.begin(), roomLists.end(),
                                          [&](Room &room)
                                          { return room.getRoomId() == roomId; });
                    if (result != roomLists.end())
                    {
                        roomLists.erase(result);
                        writeVectorToExcel(filename, roomLists);
                        cout << "Delete Room successfully!" << endl;
                    }
                    else
                    {
                        cout << "Room doesn't exist. Can't delete!" << endl;
                    }
                }
                break;
                case 6:
                {
                    int roomId;
                    printLabel("CheckOut Room");
                    cout << "Enter Room ID: ";
                    cin >> roomId;
                    auto result = find_if(roomLists.begin(), roomLists.end(),
                                          [&](Room &room)
                                          { return room.getRoomId() == roomId; });
                    if (result != roomLists.end())
                    {
                        cout << "Room ID found!" << endl;
                        Room roomUpdate = *result;
                        roomUpdate.setAvailable(1);
                        *result = roomUpdate;
                        writeVectorToExcel(filename, roomLists);
                        removeBookingFromSheet(filename, "Booked Room", roomId);
                        cout << "Successfully Checkout the room!" << endl;
                    }
                    else
                    {
                        cout << "Room Not found!" << endl;
                    }
                }
                break;
                case 7:
                    cout << "Exit from the program" << endl;
                    break;
                default:
                    cout << "Invalid option! Choose again from 1-6" << endl;
                    break;
                }
                if (adminOption != 7)
                    pressEnter();
            } while (adminOption != 7);
        }
        else if (name == "guest" && password == "guest123")
        {
            int guestOption;
            do
            {
                system("cls");
                printMenuAsTable(guestMenuList, "Guest console App");
                cout << "Please choose the option above : ";
                cin >> guestOption;

                vector<Room> freshRooms = readRoomFromExcel(filename);
                vector<Room> freshBooked = readBookingFromSheet(filename, "Booked Room");
                mergeBookingData(freshRooms, freshBooked);

                switch (guestOption)
                {
                case 1:
                {
                    printLabel("Display all Rooms");
                    Guest gs1;
                    gs1.displayRoomForGuest(freshRooms);
                }
                break;
                case 2:
                {
                    int sortOpt;
                    do
                    {
                        system("cls");
                        printLabel("Sort Option");
                        cout << "1. Sort By ID." << endl;
                        cout << "2. Sort By Price." << endl;
                        cout << "3. Exit" << endl;
                        cout << "Choose sort option above: ";
                        cin >> sortOpt;
                        switch (sortOpt)
                        {
                        case 1:
                        {
                            int sortByIdOpt;
                            system("cls");
                            printLabel("Sort By ID");
                            cout << "1. Sort from smallest to largest" << endl;
                            cout << "2. Sort from largest to smallest" << endl;
                            cout << "3. Exit" << endl;
                            cout << "Choose sort option above: ";
                            cin >> sortByIdOpt;
                            switch (sortByIdOpt)
                            {
                            case 1:
                            {
                                Guest gs1;
                                gs1.sortRoomByIdAsc(freshRooms);
                            }
                            break;
                            case 2:
                            {
                                Guest gs1;
                                gs1.sortRoomByIdDsc(freshRooms);
                            }
                            break;
                            case 3:
                                break;
                            default:
                                cout << "No option to choose" << endl;
                                break;
                            }
                        }
                        break;
                        case 2:
                        {
                            int sortByPriceOpt;
                            system("cls");
                            printLabel("Sort By Price");
                            cout << "1. Sort from smallest to largest" << endl;
                            cout << "2. Sort from largest to smallest" << endl;
                            cout << "3. Exit" << endl;
                            cout << "Choose sort option above: ";
                            cin >> sortByPriceOpt;
                            switch (sortByPriceOpt)
                            {
                            case 1:
                            {
                                Guest gs1;
                                gs1.sortRoomByPriceAsc(freshRooms);
                            }
                            break;
                            case 2:
                            {
                                Guest gs1;
                                gs1.sortRoomByPriceDsc(freshRooms);
                            }
                            break;
                            case 3:
                                break;
                            default:
                                cout << "No option to choose" << endl;
                                break;
                            }
                        }
                        break;
                        case 3:
                            break;
                        default:
                            cout << "No Sort option" << endl;
                            break;
                        }
                        if (sortOpt != 3)
                            pressEnter();
                    } while (sortOpt != 3);
                }
                break;
                case 3:
                {
                    printLabel("Booking Room Section");
                    Guest gs;
                    gs.bookingRoom(freshRooms, filename);
                }
                break;
                case 4:
                {
                    printLabel("Search Room By ID");
                    Guest gs;
                    gs.searchGuestRoomById(freshRooms);
                }
                break;
                case 5:
                {
                    printLabel("Booking Receipt History");
                    Guest gs;
                    gs.showReceipt(freshRooms);
                }
                break;
                case 6:
                    cout << "Logout from guestOption successfully!" << endl;
                    break;
                default:
                    cout << "No option to choose ! <<< Choose again >>>";
                    break;
                }
                pressEnter();
            } while (guestOption != 6);
        }
        else
        {
            cout << "Login failed! Please try again!" << endl;
        }
        pressEnter();
        countError++;
    } while (countError < 3);
    return 0;
}