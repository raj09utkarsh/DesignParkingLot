// Problem Statemenet: Design a parking lot where we have to park vehicle based on distance nearest from entrance


#include<bits/stdc++.h>
using namespace std;


class Vehicle
{
    string registrationNumber;
public:
    Vehicle() {
    }


    Vehicle(string registrationNumber) {
        this->registrationNumber = registrationNumber;
    }

    string getRegistrationNumber() {
        return this->registrationNumber;
    }

    void setRegistrationNumber(string registrationNumber) {
        this->registrationNumber = registrationNumber;
    }
};


class ParkingSlot {
public:
    int age; // Age of the driver who parked the car
    int slot; // Slot number of the parking slot
    string registrationNumber; // Registration number of the parked vehicle

    ParkingSlot() {
        this->slot = -1; // -1 indicates that the parking slot is empty
    }

    ParkingSlot(int slot) {
        this->slot = slot;
    }

    void parkVehicle(Vehicle vehicle, int slot, int age) {
        this->registrationNumber = vehicle.getRegistrationNumber();
        this->slot = slot;
        this->age = age;
    }
};


// Comparator function to implement min priority queue for ParkingSlot class
class CompareSlot {
public:
    bool operator() (ParkingSlot P1, ParkingSlot P2) {
        return P1.slot > P2.slot;
    }
};


// ParkingLot will be a class where each object will be a ParkingSlot, for this vector is used
// Min Priority Queue is used to find the ParkingSlot nearest to the entrance
class ParkingLot
{
    int size;
    int occupied;
    
    vector<ParkingSlot> slots; // Each element of this vector is a parking slot containg detail of the vehicle parked
    map<int, set<int> > ageWithSlots; // age, slot number
    map<string, int> vehicleWithSlot; // vehicle registration number, slot number
    priority_queue<ParkingSlot, vector<ParkingSlot>, CompareSlot > minEmptySlot; // min priority queue to sort by nearest

    
public:
    ParkingLot(){
    }

    ParkingLot(int size) {
        this->size = size;
        this->occupied = 0;

        // Inititlize the parking slot
        slots.clear();
        slots.resize(size + 1);

        // Add empty slots to the min priority queue
        for(int slot = 1; slot <= size; slot++) {
            ParkingSlot p = ParkingSlot(slot);
            minEmptySlot.push(p);
        }

        cout<<"Created parking for "<<size<<" slots"<<endl;
    }


    void parkVehicle(Vehicle vehicle, int age) {
        if(this->size == this->occupied) {
            cout<<"No slot empty to park vehicle."<<endl;
            return;
        }

        string registrationNumber = vehicle.getRegistrationNumber();
        ParkingSlot emptySlot = minEmptySlot.top();
        minEmptySlot.pop();

        emptySlot.parkVehicle(vehicle, emptySlot.slot, age);
        this->slots[emptySlot.slot] = emptySlot;
        this->ageWithSlots[age].insert(emptySlot.slot);
        this->vehicleWithSlot[registrationNumber] = emptySlot.slot;
        this->occupied = this->occupied + 1;

        cout<<"Car with registration number \""<<registrationNumber<<"\" has been parked at slot number "<<emptySlot.slot<<endl;
    }


    void removeVehicle(int slot) {
        if(this->occupied == 0) {
            cout<<"No vehicle parked currently in the parking lot"<<endl;
            return;
        } 

        ParkingSlot currentSlot = slots[slot];

        if(currentSlot.slot == -1) {
            cout<<"No vehicle parked at the slot number "<<slot<<endl;
            return;
        }

        slots[slot].slot = -1;
        ageWithSlots[slot].erase(currentSlot.age);
        vehicleWithSlot.erase(currentSlot.registrationNumber);
        minEmptySlot.push(slot);
        this->occupied = this->occupied - 1;

        cout<<"Slot number "<<slot<<" vacated, the car with vehicle registration number \""<<currentSlot.registrationNumber<<"\" left the space, the driver of the car was of age "<<currentSlot.age<<endl;
    }


    set<int> getSlotsFromAge(int age) {
        set<int> parkingSlots = this->ageWithSlots[age];
        return parkingSlots;
    }

    void printSlotForAge(int age) {
        set<int> slotsForAge = getSlotsFromAge(age);
        for(auto slot = slotsForAge.begin(); slot != slotsForAge.end(); slot++) {
            auto itr = slot;
            ++itr;
            if(itr == slotsForAge.end()) {
                cout<<*slot<<"";
            } else {
                cout<<*slot<<",";
            }
        }
        cout<<endl;
    }

    int getSlotFromRegistration(string registrationNumber) {
        int slot = vehicleWithSlot[registrationNumber];
        return slot;
    }

    void printRegistrationNumberFromAge(int age) {
        set<int> slotsForAge = getSlotsFromAge(age);
        if(slotsForAge.empty()) {
            cout<<"No vehicle parked with drivers of age "<<age<<endl;
            return;
        }

        for(auto slot = slotsForAge.begin(); slot != slotsForAge.end(); slot++) {
            ParkingSlot parkingSlot = slots[*slot];
            cout<<parkingSlot.registrationNumber<<", ";
        }
        cout<<endl;
    }

    void printSlotForRegistration(string registrationNumber) {
        int slot = getSlotFromRegistration(registrationNumber);
        if(slot != 0){
            cout<<slot<<endl;
        } else {
            cout<<"No vehicle parked with registration number "<<registrationNumber<<endl;
        }
    }
};


// C++ tokenizer to spilt string(sentence) into vector of string(word)
vector<string> tokenizer(string input)
{
    stringstream ss(input);
    string command;
    vector<string> commands;
    while (ss >> command) {
        commands.push_back(command);
    }
    return commands;
}



int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    ParkingLot parkingLot;

    fstream inputFile;
    inputFile.open("input.txt",ios::in);
    if (inputFile.is_open()){
        string rawCommand;
        while(getline(inputFile, rawCommand)){ 
            vector<string> command = tokenizer(rawCommand);
            
            if(command.size() == 2 && command[0] == "Create_parking_lot") {
                int size = stoi(command[1]);
                parkingLot = ParkingLot(size);
            } else if(command.size() == 4 && command[0] == "Park" && command[2] == "driver_age") {
                Vehicle vehicle;
                vehicle.setRegistrationNumber(command[1]);
                int age = stoi(command[3]);
                parkingLot.parkVehicle(vehicle, age);
            } else if(command.size() == 2 && command[0] == "Slot_numbers_for_driver_of_age") {
                int age = stoi(command[1]);
                parkingLot.printSlotForAge(age);
            } else if(command.size() == 2 && command[0] == "Slot_number_for_car_with_number") {
                string registrationNumber = command[1];
                parkingLot.printSlotForRegistration(registrationNumber);
            } else if(command.size() == 2 && command[0] == "Leave") {
                int slot = stoi(command[1]);
                parkingLot.removeVehicle(slot);
            } else if(command.size() == 2 && command[0] == "Vehicle_registration_number_for_driver_of_age") {
                int age = stoi(command[1]);
                parkingLot.printRegistrationNumberFromAge(age);
            } else {
                cout<<"Invalid Command"<<endl;
            }
        }
        inputFile.close(); 
    }    
    return 0;
}
