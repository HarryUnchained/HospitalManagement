/*
 * If you are running this program for the first time you will have to create two text files.
 * Create a file named DoctorData.txt and put a 1 at the beginning of the file.
 * Create another file named PatientData.txt and put a 1 at the beginning of the file.
 */
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

using namespace std;


void displayPatientMainMenu(); //Main menu for patients

void bookAppointment(); //Books Patient Appointment

string checkGender(string *gender); //Returns gender string based on given parameter

void displayRecords(string *patient_or_doctor); //Display all records for doctor or patient based upon which string paramater is passed to it

int searchPatientOrDoctor(string *patient_or_doctor); //Search inside Doctor or Patient structure and return a non-zero value if id found

void availableDoctors(); //List available doctors

void displayDays(int doc_selected); //List selected doctor days

void displaySlots(int doc_selected, int day_selected); //List selected doctor slots

void displayPatientOrDoctor(int id, string *patient_or_doctor); //Display Patient or Doctor info

bool isEmptyDatabase(string *patient_or_doctor); //Check if patient or doctor record exists

int getLastPatientID(); //Get last patient ID from its file

int getLastDoctorID(); //Get last doctor ID from its file

int loginScreen(); //Main login screen

void displayOnTop(); //Display clinic name on top

void displayAdminMainMenu(); //Main menu for admins

bool isPatient(string *patient_or_doctor); //Returns true if the given parameter is a patient and false otherwise

void addDoctor(); //Add a new doctor

void addDoctorDays(string* days_passed, string day_characters, int id); //Saves the days in which doctor will sit

void addDoctorSlots(int id); // Saves the slots for doctor

void deleteDoctorOrPatient(string *patient_or_doctor); //Find's and deletes a doctors or patient's record

void modifyDoctorData(); //Change the days or slots for doctor

void saveDataToFile(); //Save all patients and doctors data from their respective struct arrays to their files

void getDataFromFile(); //Fetch all patients and doctors data from file and load onto their respective struct arrays

void testRecommendation(); //Free test recommendation to the patient based upon his/her symptoms

struct PatientData {
    string name = "Empty"; //Declared empty so could later be used to identify if record exists
    int age;
    string gender;
    string doctor;
    string day;
    string slot;
};

struct Doctors {

    string doc_name = "Empty"; //Declared empty so could later be used to identify if record exists
    string clinic_days[5] = { "Booked", "Booked", "Booked", "Booked", "Booked" }; //Clinic only opens for 5 days but doctor could sit lesser than those
    string doctors_slots[5][6]; //Maximum slots for a doctor per day are 6

};


int patient_counter;
int doctor_counter;
Doctors* doctors{ new Doctors[doctor_counter + 10] {} };
PatientData* patient{ new PatientData[patient_counter + 50] {} };


int booking_doctors_left = doctor_counter;



int main() {
    char response = 'Y';
    int selection;

    patient_counter = getLastPatientID();
    doctor_counter = getLastDoctorID();
    getDataFromFile();

    do {
        displayOnTop();
        selection = loginScreen();
        if (selection == 1)  //Admin Logged In
        {
            displayAdminMainMenu();
        }
        else if (selection == 2) { //Patient Logged In
            displayPatientMainMenu();
            cin.ignore();
        }
        else if (selection == 3) {
            break;
        }
        else {
            cout << "\nSomething went wrong!\n\n";
        }
        cout << "\n\nDo you wish to exit program?(Y/N): ";
        cin >> response;
        system("CLS");

    } while (response != 'N' && response != 'n');
    displayOnTop();
    cout << "\n\nProgram executed successfully...\n\n\n";
    saveDataToFile();
    system("pause");
    return 0;
}

void displayPatientMainMenu() {
    int selection;
    char response = 'Y';

    do{
        WrongSelection:
        displayOnTop();

        cout << "1.Book Appointment.\n";
        cout << "2.Display All Records.\n";
        cout << "3.Search for a patient.\n";
        cout << "4.Display Available Doctors.\n";
        cout << "5.Get free test recommendation based on your symptoms.\n\n";
        cout << "Enter respective task number you wish to perform: ";
        cin >> selection;

        system("CLS");

        if (selection < 1 || selection > 5) {
            cout << "Task not available!";
            goto WrongSelection;
        }


        if(selection == 1) {
            if (booking_doctors_left == 0) {
                cout << "\nBookings aren't available at the moment.\n\n";
            } else { bookAppointment(); }
        }
        else if(selection == 2) {
            string patient_or_doc = "Patient";
            displayRecords(&patient_or_doc);
        }
        else if(selection == 3) {
            string patient_or_doc = "Patient";
            searchPatientOrDoctor(&patient_or_doc);
        }
        else if(selection == 4) {
            availableDoctors();
        }
        else if(selection == 5) {
            testRecommendation();
        }
        else {
            cout << "Invalid Response!\n\n";
        }
        cout << "\n\nDo you wish to perform another task?(Y/N): ";
        cin >> response;
        system("CLS");

    } while(response != 'N' && response != 'n');

}

void bookAppointment() {
    int doc_num, day_num, slot_selection;
    displayOnTop();
    cout << "_";
    cout << "\nPatient Name: ";
    cin.ignore();
    getline(cin, patient[patient_counter].name);

    cout << "\nPatient Age: ";
    cin >> patient[patient_counter].age;

    cout << "\nPatient Gender(M/F): ";
    cin >> patient[patient_counter].gender;

    patient[patient_counter].gender = checkGender(&patient[patient_counter].gender);
    cout << endl << endl;

    availableDoctors();
    if (booking_doctors_left != 0) {
        WrongDoctor:
        cout << "Enter doctor number you want an appointment for: ";
        cin >> doc_num;

        if (doc_num < 1 || doc_num > booking_doctors_left) {
            cout << "\nIncorrect doctor number selected.\n";
            goto WrongDoctor;
        }
        doc_num -= 1;
        patient[patient_counter].doctor = doctors[doc_num].doc_name;


        cout << "\nClinic Timings: Every Monday to Friday from 11AM to 3PM.\n";

        displayDays(doc_num);
        WrongDay:
        cout << "\nEnter respective number to select a day: ";
        cin >> day_num;

        if (day_num < 1 || day_num > 5) {
            cout << "\nIncorrect day number selected.\n";
            goto WrongDay;
        }
        day_num -= 1;
        patient[patient_counter].day = doctors[doc_num].clinic_days[day_num];


        displaySlots(doc_num, day_num);
        WrongSlot:
        cout << "\nEnter respective number to select a slot: ";
        cin >> slot_selection;

        if (slot_selection < 1 || slot_selection > 6) {
            cout << "\nIncorrect slot number selected.\n";
            goto WrongSlot;
        }
        slot_selection -= 1;
        patient[patient_counter].slot = doctors[doc_num].doctors_slots[day_num][slot_selection];
        doctors[doc_num].doctors_slots[day_num][slot_selection] = "Booked";


        system("CLS");
        string patient_or_doc = "Patient";
        displayPatientOrDoctor(patient_counter,&patient_or_doc);
        patient_counter++;
    }
}

string checkGender(string *gender) {
    if (*gender == "M" || *gender == "m" || *gender == "male" || *gender == "Male") {
        return "Male";
    }
    else {
        return "Female";
    }
}

void displayRecords(string *patient_or_doctor) {

    if (isPatient(patient_or_doctor)) {
        if (!isEmptyDatabase(patient_or_doctor)) {
            for (int i = 0; i <= patient_counter; i++) {
                cout << " _";
                cout << "\nPatient ID : " << i;
                cout << "\nPatient Name : " << patient[i].name;
                cout << "\nPatient Age : " << patient[i].age;
                cout << "\nPatient Gender : " << patient[i].gender;
                cout << "\nAppointment with doctor : " << patient[i].doctor;
                cout << "\nOn: (" << patient[i].day << ")";
                cout << "\tAt: (" << patient[i].slot << ")";
                cout << "\n__";
                cout << endl << endl << endl;
            }
        }
    }
    else {
        if (!isEmptyDatabase(patient_or_doctor)) {
            for (int i = 0; i <= doctor_counter; i++) {
                cout << " __";
                cout << "\nDoctor ID : " << i;
                cout << "\nDoctor Name : " << doctors[i].doc_name;
                cout << "\nDoctor Days : " << doctors[i].clinic_days;
                cout << "\nDoctor Slots : " << doctors[i].doctors_slots;
                cout << "\n___";
                cout << endl << endl << endl;
            }
        }
    }

}

int searchPatientOrDoctor(string *patient_or_doctor) {
    bool found = false;
    int search_id;

    if (isPatient(patient_or_doctor)) {

        if (!isEmptyDatabase(patient_or_doctor)) {

            WrongInput:
            cout << "\n___";
            cout << "\nEnter Patient ID you want to search: ";
            cin >> search_id;
            cout << "___\n";
            if (search_id < 0 || search_id >= 'A' && search_id <= 'Z' || search_id >= 'a' && search_id <= 'z') {
                cout << "\nIncorrect ID entered!\n";
                cout << "_\n";
                goto WrongInput;
            }
            else {
                for (int i = 0; i <= patient_counter; i++) {
                    if (i == search_id) {
                        displayPatientOrDoctor(i, patient_or_doctor);
                        found = true;
                    }
                }
                if (!found) {
                    cout << "\nNo patient found at the given id.";
                    return 0;
                }
                else {
                    return search_id;
                }
            }
        }
    }
    else {
        if (!isEmptyDatabase(patient_or_doctor)) {

            WrongInputForDoc:
            cout << "\n___";
            cout << "\nEnter Doctor ID you want to search: ";
            cin >> search_id;
            cout << "___\n";
            if (search_id < 0 || search_id >= 'A' && search_id <= 'Z' || search_id >= 'a' && search_id <= 'z') {
                cout << "\nIncorrect ID entered!\n";
                cout << "_\n";
                goto WrongInputForDoc;
            }
            else {
                for (int i = 0; i <= doctor_counter; i++) {
                    if (i == search_id) {
                        displayPatientOrDoctor(i, patient_or_doctor);
                        found = true;
                    }
                }
                if (!found) {
                    cout << "\nNo doctor found at the given id.";
                    return 0;
                }
                else {
                    return search_id;
                }
            }
        }
    }
    return 0;
}

void availableDoctors() {
    booking_doctors_left = 0;
    displayOnTop();
    cout << "_\n";
    cout << "\nAvailable Doctors:";
    for (int i = 0; doctors[i].doc_name != "Empty"; i++) {
        if (doctors[i].doc_name != "Booked") {
            booking_doctors_left++;
            cout << "\t(" << i + 1 << ") " << doctors[i].doc_name;
        }
    }
    if (booking_doctors_left == 0) {
        cout << "\nWe are sorry no doctors are available for the next 5 days.\n\n";
    }

    cout << "\n___";
    cout << endl << endl;
}

void displayDays(int doc_selected) {
    int booking_days_left = 0;

    cout << "__";
    cout << "\nSelect day:";
    for (int i = 0; i < 5; i++) {
        if (doctors[doc_selected].clinic_days[i] == "Booked") {}

        else {
            booking_days_left++;
            cout << "\t(" << i + 1 << ") " << doctors[doc_selected].clinic_days[i];
        }
    }
    if (booking_days_left <= 1) {
        doctors[doc_selected].doc_name = "Booked";
    }

    cout << "\n__";
    cout << endl << endl;
}

void displaySlots(int doc_selected, int day_selected) {

    int booking_slots_left = 0;

    cout << "_";
    cout << "\nSelect Slot:";
    for (int i = 0; i < 6; i++) {

        if (doctors[doc_selected].doctors_slots[day_selected][i] == "Booked") {}

        else {
            booking_slots_left++;
            cout << "\t(" << i + 1 << ") " << doctors[doc_selected].doctors_slots[day_selected][i];
        }
    }
    if (booking_slots_left <= 1) {
        doctors[doc_selected].clinic_days[day_selected] = "Booked";
    }

    cout << "\n___";
    cout << endl << endl;
}

void displayPatientOrDoctor(int id, string *patient_or_doctor) {
    system("CLS");
    displayOnTop();
    if (isPatient(patient_or_doctor)) {
        cout << "\n___";
        cout << "\nPatient ID: " << id;
        cout << "\nPatient Name: " << patient[id].name;
        cout << "\nPatient Age: " << patient[id].age;
        cout << "\nPatient Gender: " << patient[id].gender;
        cout << "\nAppointment with doctor: " << patient[id].doctor;
        cout << "\nOn: (" << patient[id].day << ")";
        cout << "\tAt: (" << patient[id].slot << ")";
        cout << "\n___";
    }
    else {
        cout << " __";
        cout << "\nDoctor ID : " << id;
        cout << "\nDoctor Name : " << doctors[id].doc_name;
        cout << "\nDoctor Days : " << doctors[id].clinic_days;
        cout << "\nDoctor Slots : " << doctors[id].doctors_slots;
        cout << "\n___";
    }
}

bool isEmptyDatabase(string *patient_or_doctor) {
    displayOnTop();
    if (isPatient(patient_or_doctor)) {
        if (patient[0].name == "Empty") {
            cout << "\nNo records found to display.\n";
            return true;
        }
        else {
            return false;
        }
    }
    else {
        if (doctors[0].doc_name == "Empty") {
            cout << "\nNo records found to display.\n";
            return true;
        }
        else {
            return false;
        }
    }

}

int getLastPatientID() {
    string sLine;
    int count = 0;

    ifstream infile("PatientData.txt");

    if (infile.good())
    {
        getline(infile, sLine);
        cout << "\nBefore converting patient data from the file: " << sLine << endl;
        count = stoi(sLine);
        cout << "\nAfter converting patient data from the file: " << count << endl;
    }
    else {
        cout << "\n\nSomething went wrong while opening file PatientData.txt.\n\n";
    }
    return count;
}

int getLastDoctorID() {
    string sLine;
    int count = 0;

    ifstream infile("DoctorData.txt");

    if (infile.good())
    {
        getline(infile, sLine);
        cout << "\nBefore converting doctor data from the file: " << sLine << endl;
        count = stoi(sLine);
        cout << "\nAfter converting doctor data from the file: " << count << endl;
    }
    else {
        cout << "\n\nSomething went wrong while opening file DoctorData.txt.\n\n";
    }
    return count;

}

int loginScreen() {

    int selection;
    WrongSelection:
    cout << "\n\n||" << setfill('-') << setw(63) << "Login" << setfill('-') << setw(55) << "||";
    cout << "\n\n1.Admin Login";
    cout << "\n2.Patient Login";
    cout << "\n3.Exit Program\n\n";

    cout << "Enter respective task number you wish to perform: ";
    cin >> selection;


    system("CLS");

    if (selection >=1 && selection <= 3) {}
    else {
        cout << "Task not available!\n\n\n";
        goto WrongSelection;
    }

    return selection;
}

void displayOnTop() {
    cout << "\n\n||" << setfill('-') << setw(73) << "Clinic Application C++" << setfill('-') << setw(45) << "||";
    cout << endl << endl;
}

void displayAdminMainMenu() {
    displayOnTop();
    char response;
    string username, password;
    int selection;
    do {
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

    } while (username != "clinicAdmin" && password != "pass123");
    system("CLS");
    displayOnTop();
    cout << "Admin Verified\n";
    cout << "You may proceed...\n\n\n";
    system("pause");
    system("CLS");


    do {
        displayOnTop();
        WrongSelection:
        cout << "1.Add a new doctors data.\n";
        cout << "2.Delete a doctors data.\n";
        cout << "3.Modify a doctors data.\n";
        cout << "4.Search for a doctors data.\n";
        cout << "5.Display all doctors data.\n";
        cout << "6.Delete a patient's record.\n\n\n";

        cout << "Enter respective task number you wish to perform: ";
        cin >> selection;
        cin.ignore();
        system("CLS");

        string patient_or_doctor = "Doctor";

        if (selection < 1 || selection > 6) {
            cout << "Task not available!\n\n\n";
            goto WrongSelection;
        }

        if(selection == 1) {
            addDoctor();
        }
        else if(selection == 2) {
            patient_or_doctor = "Doctor";
            deleteDoctorOrPatient(&patient_or_doctor);
        }
        else if(selection == 3) {
            modifyDoctorData();
        }
        else if(selection == 4) {
            patient_or_doctor = "Doctor";
            searchPatientOrDoctor(&patient_or_doctor);
        }
        else if(selection == 5) {
            patient_or_doctor = "Doctor";
            displayRecords(&patient_or_doctor);
        }
        else if(selection == 6) {
            patient_or_doctor = "Patient";
            deleteDoctorOrPatient(&patient_or_doctor);
        }
        else {
            cout << "\nSomething went wrong...";
        }
        cout << "\n\nDo you wish to perform another task?(Y/N): ";
        cin >> response;
        system("CLS");
    } while (response == 'y' || response == 'Y');
}

bool isPatient(string *patient_or_doctor) {
    if (*patient_or_doctor == "Patient" || *patient_or_doctor == "patient") {
        return true;
    }
    else {
        return false;
    }
}

void addDoctor() {
    displayOnTop();
    string day;
    string days[] = { "Monday", "Tuesday",	"Wednesday", "Thursday", "Friday" };
    cout << "Doctor Name: ";
    getline(cin, doctors[doctor_counter].doc_name);

    cout << "Select Doctor Days: (Monday)\t(Tuesday)\t(Wednesday)\t(Thursday)\t(Friday)\n";
    cout << "Enter first character of the days in a string to select a day e.g: MTW would mean Monday Tuesday and Wednesday are selected.\n";
    cout << "Use Th for Thursday and T for Tuesday.";

    cout << "Enter here: ";
    cin >> day;
    cin.ignore();

    addDoctorDays(days, day, doctor_counter);
    addDoctorSlots(doctor_counter);
    doctor_counter++;

}

void addDoctorDays(string* days_passed, string day_characters, int id) {

    for (int i = 0; i < day_characters.length(); i++) {
        if (day_characters[i] == 'M') {
            doctors[id].clinic_days[0] = days_passed[0];
        }
        else if (day_characters[i] == 'T') {
            doctors[id].clinic_days[1] = days_passed[1];
        }
        else if (day_characters[i] == 'W') {
            doctors[id].clinic_days[2] = days_passed[2];
        }
        else if (day_characters[i] == 'T' && day_characters[i + 1] == 'h') {
            doctors[id].clinic_days[3] = days_passed[3];
        }
        else if (day_characters[i] == 'F') {
            doctors[id].clinic_days[4] = days_passed[4];
        }
    }


}

void addDoctorSlots(int id) {
    cout << "\nThe timings for all days will be the same.\n";
    cout << "There can only be 6 slots per doctor.\n";
    cout << "You must enter timings in the following format: 11:00 AM, 01:00 PM\n\n";
    cout << "If you want to skip a slot input skip.\n\n";
    string timings[6];

    for (int i = 0; i < 6; i++) {
        cout << "Enter here: ";
        getline(cin,timings[i]);
        if (timings[i] == "skip" || timings[i] == "Skip") {
            timings[i] = "Booked";
        }
    }
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 6; j++) {
            doctors[id].doctors_slots[i][j] = timings[j];
        }
    }
}

void deleteDoctorOrPatient(string *patient_or_doctor) {
    int id = 0;
    char response;
    if (isPatient(patient_or_doctor)) {
        id = searchPatientOrDoctor(patient_or_doctor);
        if (id) {
            cout << "\nAre you sure you want to delete this record? (Y/N): ";
            cin >> response;
            if (response == 'Y' || response == 'y') {
                cout << "Deleting record...";
                for (int i = id; i <= patient_counter; i++) {
                    cout << "....";
                    patient[i].name = patient[i + 1].name;
                    patient[i].age = patient[i + 1].age;
                    patient[i].gender = patient[i + 1].gender;
                    patient[i].doctor = patient[i + 1].doctor;
                    patient[i].day = patient[i + 1].day;
                    patient[i].slot = patient[i + 1].slot;
                }
                patient_counter--;
            }
        }
        else {
            return;
        }
    }
    else {
        id = searchPatientOrDoctor(patient_or_doctor);
        if (id) {
            cout << "\nAre you sure you want to delete this record? (Y/N): ";
            cin >> response;
            if (response == 'Y' || response == 'y') {
                cout << "Deleting record...";
                for (int i = id; i <= doctor_counter; i++) {
                    cout << "....";
                    doctors[i].doc_name = doctors[i + 1].doc_name;
                    for (int j = 0; j < 5; j++) {
                        doctors[i].clinic_days[j] = doctors[i + 1].clinic_days[j];
                        for (int k = 0; k < 6; k++) {
                            doctors[i].doctors_slots[j][k] = doctors[i + 1].doctors_slots[j][k];
                        }
                    }
                }
                doctor_counter--;
            }
        }
        else {
            return;
        }
    }


}

void modifyDoctorData() {
    string day;
    string days[] = { "Monday", "Tuesday",	"Wednesday", "Thursday", "Friday" };
    int choice;
    int id = 0;
    string patient_or_doc = "Doctor";
    id = searchPatientOrDoctor(&patient_or_doc);
    if (id) {
        WrongChoiceSelected:
        displayPatientOrDoctor(id, &patient_or_doc);
        cout << "\n1.Modify days";
        cout << "\n2.Modify slots";
        cout << "\n3.Modify both";

        cout << "\nEnter choice number: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Select Doctor Days: (Monday)\t(Tuesday)\t(Wednesday)\t(Thursday)\t(Friday)\n";
                cout << "Enter first character of the days in a string to select a day e.g: MTW would mean Monday Tuesday and Wednesday are selected.\n";
                cout << "Use Th for Thursday and T for Tuesday.";

                cout << "Enter here: ";
                cin >> day;
                addDoctorDays(days, day, id);
                break;

            case 2:
                addDoctorSlots(id);
                break;
            case 3:
                cout << "Select Doctor Days: (Monday)\t(Tuesday)\t(Wednesday)\t(Thursday)\t(Friday)\n";
                cout << "Enter first character of the days in a string to select a day e.g: MTW would mean Monday Tuesday and Wednesday are selected.\n";
                cout << "Use Th for Thursday and T for Tuesday.\n";

                cout << "Enter here: ";
                cin >> day;
                addDoctorDays(days, day, id);
                addDoctorSlots(id);
                break;

            default:
                cout << "Wrong choice selected try again...\n";
                system("pause");
                goto WrongChoiceSelected;
                break;
        }
    }
}

void saveDataToFile() {
    fstream SavePatientFile;
    SavePatientFile.open("PatientData.txt", ios::out);

    if (SavePatientFile.is_open()) {
        SavePatientFile << patient_counter << endl;
        for (int i = 0; i <= patient_counter; i++) {
            SavePatientFile << patient[i].name << endl;
            SavePatientFile << patient[i].age << endl;
            SavePatientFile << patient[i].gender << endl;
            SavePatientFile << patient[i].doctor << endl;
            SavePatientFile << patient[i].day << endl;
            SavePatientFile << patient[i].slot << endl;
        }
        SavePatientFile.close();
    }
    else {
        cout << "\n\nCouldn't open PatientData.txt!\n";
    }
    fstream SaveDoctorFile;
    SaveDoctorFile.open("DoctorData.txt", ios::out);
    if (SaveDoctorFile.is_open()) {
        SaveDoctorFile << doctor_counter << endl;
        for (int i = 0; i <= doctor_counter; i++) {
            SaveDoctorFile << doctors[i].doc_name << endl;
            for (int j = 0; j < 5; j++) {
                SaveDoctorFile << doctors[i].clinic_days[j] << " ";
            }
            SaveDoctorFile << endl;
            for (int j = 0; j < 5; j++) {
                for (int k = 0; k < 6; k++) {
                    SaveDoctorFile << doctors[j].doctors_slots[j][k] << " ";
                }
                SaveDoctorFile << endl;
            }
        }
    }
    else {
        cout << "\n\nCouldn't open DoctorData.txt!\n";
        return;
    }
}

void getDataFromFile() {
    string junk;

    fstream GetPatientFile;
    GetPatientFile.open("PatientData.txt", ios::in);

    if (GetPatientFile.is_open()) {
        GetPatientFile >> junk;
        for (int i = 0; i <= patient_counter; i++) {
            GetPatientFile >> patient[i].name;
            GetPatientFile >> patient[i].age;
            GetPatientFile >> patient[i].gender;
            GetPatientFile >> patient[i].doctor;
            GetPatientFile >> patient[i].day;
            GetPatientFile >> patient[i].slot;
        }
        GetPatientFile.close();
    }
    else {
        cout << "\n\nCouldn't open PatientData.txt!\n";
    }

    fstream GetDoctorFile;
    GetDoctorFile.open("DoctorData.txt", ios::in);
    if (GetDoctorFile.is_open()) {

        GetDoctorFile >> junk;
        for (int i = 0; i <= doctor_counter; i++) {
            GetDoctorFile >> doctors[i].doc_name;
            for (int j = 0; j < 5; j++) {
                GetDoctorFile >> doctors[i].clinic_days[j];
            }
            GetDoctorFile << endl;
            for (int j = 0; j < 5; j++) {
                for (int k = 0; k < 6; k++) {
                    GetDoctorFile >> doctors[j].doctors_slots[j][k];
                }
            }
        }
    }
    else {
        cout << "\n\nCouldn't open DoctorData.txt!\n";
        return;
    }

}

void testRecommendation() {

    int selection;
    do {
        displayOnTop();
        cout << endl;
        cout << "1) Heart Ache\n";
        cout << "2) Headache\n";
        cout << "3) Flu, Sore Throat, Fever\n";
        cout << "4) Intestine Pain\n";
        cout << "5) Body Pain\n";

        cout << "\n\nEnter Respective Number Of Symptoms: ";
        cin >> selection;

        cout << "\nTest Recommendation According To Your Symptoms: ";

        switch (selection) {
            case 1:
                cout << "ECG\n";
                break;

            case 2:
                cout << "MRI, CT SCAN\n";
                break;

            case 3:
                cout << "Covid 19\n";
                break;

            case 4:
                cout << "Blood Test\n";
                break;

            case 5:
                cout << "Vitamin D, X-ray, Uric Acid\n";
                break;

            default:
                cout << "\n\nInvalid selection...\n\n";
                break;
        }
    }while(selection < 1 || selection > 5);
}