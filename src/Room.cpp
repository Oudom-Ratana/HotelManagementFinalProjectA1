#include<Room.hpp>
#include<iostream>

using namespace std;
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

    // void output()
    // {
    //     cout << id << "-" << name << "-" << gender << "-" << average << endl;
    // }
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

    string getName()
    {
        return name;
    }
    string getGender()
    {
        return gender;
    }
    int getPhoneNum()
    {
        return phoneNum;
    }
    string getCheckInDate()
    {
        return checkInDate;
    }
    int getRoomId()
    {
        return roomId;
    }
    string getRoomType()
    {
        return roomType;
    }
    string getEquipments()
    {
        return equipments;
    }
    float getPricePerNight()
    {
        return pricePerNight;
    }
    int getAvailability()
    {
        return isAvailable;
    }

    void setRoomId(int roomId) { this->roomId = roomId; }
    void setRoomType(string roomType) { this->roomType = roomType; }
    void setEquipment(string equipments) { this->equipments = equipments; }
    void setPricePerNight(float pricePerNight)
    {
        this->pricePerNight = pricePerNight;
    }
    void setAvailable(int isAvailable)
    {
        this->isAvailable = isAvailable;
    }
    string getAvailabilityString()
    {
        if (isAvailable == 1)
            return "Available";
        if (isAvailable == 0)
            return "Booked";

        return "Unknown";
    }
};
