#ifndef ROOM_HPP
#define ROOM_HPP

#include <iostream>
#include <string>
#include <vector>
#include <tabulate/table.hpp>

using namespace std;
using namespace tabulate;

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

    Room();
    Room(int roomId, string roomtype, string equipments, float pricePerNight, int isAvailable);
    Room(string name, string gender, int phoneNum, string checkInDate, string checkOutDate,
         int roomId, string roomtype, string equipments, float pricePerNight, int isAvailable);

    void input();
    void update();

    string getName();
    string getGender();
    int getPhoneNum();
    string getCheckInDate();
    string getCheckOutDate();
    int getRoomId();
    string getRoomType();
    string getEquipments();
    float getPricePerNight();
    int getAvailability();
    string getAvailabilityString();

    void setRoomId(int roomId);
    void setRoomType(string roomType);
    void setEquipment(string equipments);
    void setPricePerNight(float pricePerNight);
    void setAvailable(int isAvailable);
};

class Guest : public Room
{
public:
    void bookingRoom(vector<Room> &roomLists, string &filename);
    void showReceipt(vector<Room> &roomLists);
    void searchGuestRoomById(vector<Room> &roomLists);
    void displayRoomForGuest(vector<Room> &roomlists);
    void sortRoomByIdAsc(vector<Room> roomLists);
    void sortRoomByIdDsc(vector<Room> roomLists);
    void sortRoomByPriceAsc(vector<Room> roomLists);
    void sortRoomByPriceDsc(vector<Room> roomLists);
};

class Admin
{
public:
    void displayRoom(vector<Room> &roomLists);
    void searchRoomById(vector<Room> &roomLists);
    void sortRoomByIdAsc(vector<Room> roomLists);
    void sortRoomByIdDsc(vector<Room> roomLists);
    void sortRoomByPriceAsc(vector<Room> roomLists);
    void sortRoomByPriceDsc(vector<Room> roomLists);
};

#endif