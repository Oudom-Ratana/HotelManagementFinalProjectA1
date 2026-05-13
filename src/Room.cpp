#include "../include/Room.hpp"
#include "../include/ExcelUtil.hpp"
#include "../include/Color.hpp"
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

Room::Room() {}

Room::Room(int roomId, string roomtype, string equipments, float pricePerNight, int isAvailable)
{
    this->roomId = roomId;
    this->roomType = roomtype;
    this->equipments = equipments;
    this->pricePerNight = pricePerNight;
    this->isAvailable = isAvailable;
}

Room::Room(string name, string gender, int phoneNum, string checkInDate, string checkOutDate,
           int roomId, string roomtype, string equipments, float pricePerNight, int isAvailable)
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

void Room::input()
{
    printColored("Enter roomID: ", CYAN);
    cin >> roomId;
    cin.ignore();
    printColored("Enter Room Type(Single, Double, VIPsingle, VIPdouble): ", CYAN);
    cin >> roomType;
    cin.ignore();
    printColored("Enter Equipments: ", CYAN);
    getline(cin, equipments);
    printColored("Enter price/Night: ", CYAN);
    cin >> pricePerNight;
}

void Room::update()
{
    cin.ignore();
    printColored("Enter Room Type(Single, Double, VIPsingle, VIPdouble): ", CYAN);
    cin >> roomType;
    printColored("Enter Equipments: ", CYAN);
    cin.ignore();
    getline(cin, equipments);
    printColored("Enter price/Night: ", CYAN);
    cin >> pricePerNight;
}

string Room::getName() { return name; }
string Room::getGender() { return gender; }
int Room::getPhoneNum() { return phoneNum; }
string Room::getCheckInDate() { return checkInDate; }
string Room::getCheckOutDate() { return checkOutDate; }
int Room::getRoomId() { return roomId; }
string Room::getRoomType() { return roomType; }
string Room::getEquipments() { return equipments; }
float Room::getPricePerNight() { return pricePerNight; }
int Room::getAvailability() { return isAvailable; }

void Room::setRoomId(int roomId) { this->roomId = roomId; }
void Room::setRoomType(string roomType) { this->roomType = roomType; }
void Room::setEquipment(string equipments) { this->equipments = equipments; }
void Room::setPricePerNight(float pricePerNight) { this->pricePerNight = pricePerNight; }
void Room::setAvailable(int isAvailable) { this->isAvailable = isAvailable; }

string Room::getAvailabilityString()
{
    if (isAvailable == 1) return "Available";
    if (isAvailable == 0) return "Booked";
    return "Unknown";
}

void Guest::bookingRoom(vector<Room> &roomLists, string &filename)
{
    cin.ignore();
    printColored("Enter Your Name: ", CYAN);
    getline(cin, name);
    printColored("Enter Your gender: ", CYAN);
    getline(cin, gender);
    printColored("Enter Your Phone Number: ", CYAN);
    cin >> phoneNum;
    printColored("Enter Room ID: ", CYAN);
    cin >> roomIdBook;
    cin.ignore();
    printColored("Enter CheckIn Date(dd/mm/yyyy): ", CYAN);
    getline(cin, checkInDate);
    printColored("Enter CheckOut Date(dd/mm/yyyy): ", CYAN);
    getline(cin, checkOutDate);
    printColored("Comfirm Booking?(Y/N): ", YELLOW);
    cin >> comfirmBooking;
    if (comfirmBooking == 'y' || comfirmBooking == 'Y')
    {
        auto result = find_if(roomLists.begin(), roomLists.end(),
                              [&](Room &room) { return room.getRoomId() == roomIdBook && room.getAvailability() == 1; });
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
            printSuccess("Successfully Booked the room!");
        }
        else
        {
            printError("Room Not found!");
        }
    }
    else
    {
        printWarning("Booking Cancelled!");
    }
}

void Guest::showReceipt(vector<Room> &roomLists)
{
    auto result = find_if(roomLists.begin(), roomLists.end(),
                          [&](Room &room) { return room.getAvailability() == 0; });
    if (result != roomLists.end())
    {
        int guestPhone;
        printColored("Enter your phone number to see your receipt: ", CYAN);
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
            printError("No booking found for phone number " + to_string(guestPhone));
        }
    }
    else
    {
        printWarning("No Room Booked !!!");
    }
}

void Guest::searchGuestRoomById(vector<Room> &roomLists)
{
    int roomId;
    printColored("Enter Room ID: ", CYAN);
    cin >> roomId;
    auto result = find_if(roomLists.begin(), roomLists.end(),
                          [&](Room &room) { return room.getRoomId() == roomId && room.getAvailability() == 1; });
    if (result != roomLists.end())
    {
        printSuccess("Room  <" + to_string(roomId) + "> Found!");
        Table table;
        table.add_row({"RoomID", "RoomType", "Equipments", "Price/night($)", "Availability"});
        stringstream ss;
        Room room = *result;
        ss << fixed << setprecision(2) << room.getPricePerNight();
        string priceStr = ss.str();
        string avail = room.getAvailabilityString();
        if (avail == "Available")
            avail = GREEN + avail + RESET;
        else
            avail = RED + avail + RESET;
        table.add_row({to_string(room.getRoomId()), room.getRoomType(), room.getEquipments(), priceStr, avail});
        table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
        cout << table << endl;
    }
    else
    {
        printError("Room Not Found!");
    }
}

void Guest::displayRoomForGuest(vector<Room> &roomlists)
{
    Table table;
    table.add_row({"RoomID", "RoomType", "Equipments", "Price/night($)", "Availability"});
    for (auto rooms : roomlists)
    {
        if (rooms.getAvailabilityString() == "Available")
        {
            stringstream ss;
            ss << fixed << setprecision(2) << rooms.getPricePerNight();
            string priceStr = ss.str();
            string avail = GREEN + rooms.getAvailabilityString() + RESET;
            table.add_row({to_string(rooms.getRoomId()), rooms.getRoomType(), rooms.getEquipments(), priceStr, avail});
        }
    }
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void Guest::sortRoomByIdAsc(vector<Room> roomLists)
{
    sort(roomLists.begin(), roomLists.end(),
         [&](Room &room1, Room &room2) { return room1.getRoomId() < room2.getRoomId(); });
    displayRoomForGuest(roomLists);
}

void Guest::sortRoomByIdDsc(vector<Room> roomLists)
{
    sort(roomLists.begin(), roomLists.end(),
         [&](Room &room1, Room &room2) { return room1.getRoomId() > room2.getRoomId(); });
    displayRoomForGuest(roomLists);
}

void Guest::sortRoomByPriceAsc(vector<Room> roomLists)
{
    sort(roomLists.begin(), roomLists.end(),
         [&](Room &room1, Room &room2) { return room1.getPricePerNight() < room2.getPricePerNight(); });
    displayRoomForGuest(roomLists);
}

void Guest::sortRoomByPriceDsc(vector<Room> roomLists)
{
    sort(roomLists.begin(), roomLists.end(),
         [&](Room &room1, Room &room2) { return room1.getPricePerNight() > room2.getPricePerNight(); });
    displayRoomForGuest(roomLists);
}

void Admin::displayRoom(vector<Room> &roomLists)
{
    Table table;
    table.add_row({"RoomID", "RoomType", "Equipments", "Price/night($)", "Availability"});
    for (auto rooms : roomLists)
    {
        stringstream ss;
        ss << fixed << setprecision(2) << rooms.getPricePerNight();
        string priceStr = ss.str();
        string avail = rooms.getAvailabilityString();
        if (avail == "Available")
            avail = GREEN + avail + RESET;
        else
            avail = RED + avail + RESET;
        table.add_row({to_string(rooms.getRoomId()), rooms.getRoomType(), rooms.getEquipments(), priceStr, avail});
    }
    table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
    cout << table << endl;
}

void Admin::searchRoomById(vector<Room> &roomLists)
{
    int roomId;
    printColored("Enter Room ID: ", CYAN);
    cin >> roomId;
    auto result = find_if(roomLists.begin(), roomLists.end(),
                          [&](Room &room) { return room.getRoomId() == roomId; });
    if (result != roomLists.end())
    {
        printSuccess("Room  <" + to_string(roomId) + "> Found!");
        Table table;
        table.add_row({"RoomID", "RoomType", "Equipments", "Price/night($)", "Availability"});
        stringstream ss;
        Room room = *result;
        ss << fixed << setprecision(2) << room.getPricePerNight();
        string priceStr = ss.str();
        string avail = room.getAvailabilityString();
        if (avail == "Available")
            avail = GREEN + avail + RESET;
        else
            avail = RED + avail + RESET;
        table.add_row({to_string(room.getRoomId()), room.getRoomType(), room.getEquipments(), priceStr, avail});
        table[0].format().font_style({FontStyle::bold}).font_align({FontAlign::center});
        cout << table << endl;
    }
    else
    {
        printError("Room Not Found!");
    }
}

void Admin::sortRoomByIdAsc(vector<Room> roomLists)
{
    sort(roomLists.begin(), roomLists.end(),
         [&](Room &room1, Room &room2) { return room1.getRoomId() < room2.getRoomId(); });
    displayRoom(roomLists);
}

void Admin::sortRoomByIdDsc(vector<Room> roomLists)
{
    sort(roomLists.begin(), roomLists.end(),
         [&](Room &room1, Room &room2) { return room1.getRoomId() > room2.getRoomId(); });
    displayRoom(roomLists);
}

void Admin::sortRoomByPriceAsc(vector<Room> roomLists)
{
    sort(roomLists.begin(), roomLists.end(),
         [&](Room &room1, Room &room2) { return room1.getPricePerNight() < room2.getPricePerNight(); });
    displayRoom(roomLists);
}

void Admin::sortRoomByPriceDsc(vector<Room> roomLists)
{
    sort(roomLists.begin(), roomLists.end(),
         [&](Room &room1, Room &room2) { return room1.getPricePerNight() > room2.getPricePerNight(); });
    displayRoom(roomLists);
}