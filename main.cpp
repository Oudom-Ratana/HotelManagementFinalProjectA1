#include "include/Room.hpp"
#include "include/ExcelUtil.hpp"
#include "include/MenuUtils.hpp"
#include "include/Color.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

int main()
{
    enableAnsiColors();
    system("cls");
    printBanner();

    vector<Room> roomLists;
    string filename = "roomListsStorage.xlsx";

    roomLists = readRoomFromExcel(filename);
    if (roomLists.empty())
    {
        printInfo("No existing data. Creating default rooms...");
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
        printBanner();
        printColored("Enter Username: ", CYAN);
        cin >> name;
        printColored("Enter Password: ", CYAN);
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
                printColored("Choose your option(1-7): ", YELLOW);
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
                    printSuccess("New Room added successfully!");
                }
                break;
                case 2:
                {
                    int roomId;
                    printLabel("Update Room");
                    printColored("Enter Room ID: ", CYAN);
                    cin >> roomId;
                    auto result = find_if(roomLists.begin(), roomLists.end(),
                                          [&](Room &room) { return room.getRoomId() == roomId; });
                    if (result != roomLists.end())
                    {
                        printSuccess("Room ID found!");
                        Room roomUpdate = *result;
                        roomUpdate.update();
                        *result = roomUpdate;
                        writeVectorToExcel(filename, roomLists);
                        printSuccess("Successfully Updated the room!");
                    }
                    else
                    {
                        printError("Room Not found!");
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
                    printLabel("Delete Room by ID");
                    printColored("Enter Student ID: ", CYAN);
                    cin >> roomId;
                    auto result = find_if(roomLists.begin(), roomLists.end(),
                                          [&](Room &room) { return room.getRoomId() == roomId; });
                    if (result != roomLists.end())
                    {
                        roomLists.erase(result);
                        writeVectorToExcel(filename, roomLists);
                        printSuccess("Delete Room successfully!");
                    }
                    else
                    {
                        printError("Room doesn't exist. Can't delete!");
                    }
                }
                break;
                case 6:
                {
                    int roomId;
                    printLabel("CheckOut Room");
                    printColored("Enter Room ID: ", CYAN);
                    cin >> roomId;
                    auto result = find_if(roomLists.begin(), roomLists.end(),
                                          [&](Room &room) { return room.getRoomId() == roomId; });
                    if (result != roomLists.end())
                    {
                        printSuccess("Room ID found!");
                        Room roomUpdate = *result;
                        roomUpdate.setAvailable(1);
                        *result = roomUpdate;
                        writeVectorToExcel(filename, roomLists);
                        removeBookingFromSheet(filename, "Booked Room", roomId);
                        printSuccess("Successfully Checkout the room!");
                    }
                    else
                    {
                        printError("Room Not found!");
                    }
                }
                break;
                case 7:
                    printInfo("Exit from the program");
                    break;
                default:
                    printError("Invalid option! Choose again from 1-6");
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
                printColored("Please choose the option above : ", YELLOW);
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
                        printColored("Choose sort option above: ", YELLOW);
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
                            printColored("Choose sort option above: ", YELLOW);
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
                                printError("No option to choose");
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
                            printColored("Choose sort option above: ", YELLOW);
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
                                printError("No option to choose");
                                break;
                            }
                        }
                        break;
                        case 3:
                            break;
                        default:
                            printError("No Sort option");
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
                    printInfo("Logout from guestOption successfully!");
                    break;
                default:
                    printError("No option to choose ! <<< Choose again >>>");
                    break;
                }
                pressEnter();
            } while (guestOption != 6);
        }
        else
        {
            printError("Login failed! Please try again!");
        }
        pressEnter();
        countError++;
    } while (countError < 3);

    return 0;
}