#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CARS 100
#define MAX_CUSTOMERS 100
#define MAX_RENTALS 100

#define CAR_FILE "cars.txt"
#define CUSTOMER_FILE "customers.txt"
#define RENTAL_FILE "rentals.txt"

typedef struct {
    int id;
    char brand[64];
    char model[64];
    char plate[32];
    double rentPerDay;
    int isAvailable;
    char damageReport[256]; // NEW FIELD
} Car;

typedef struct {
    int id;
    char name[64];
    char address[128];
    char phone[32];
    char license[32];
} Customer;

typedef struct {
    int id;
    int carId;
    int customerId;
    int rentDays;
    double totalCost;
    int returned;
    char rentDate[20];
    char returnDate[20];
    double lateFee;
} Rental;

Car cars[MAX_CARS];
Customer customers[MAX_CUSTOMERS];
Rental rentals[MAX_RENTALS];

int carCount = 0, customerCount = 0, rentalCount = 0;

/* Utility Functions */
void clearInputBuffer() {
    while (getchar() != '\n');
}

void pausemsg() {
    printf("\nPress Enter to continue...");
    getchar();
}

void trimNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
}

void safeInput(char *buf, int size) {
    fgets(buf, size, stdin);
    trimNewline(buf);
}

/* Autosave System */
void saveData();
void loadData();
void autoSave() { saveData(); }

/* ===================== CAR MANAGEMENT ===================== */

void addCar() {
    Car c;
    c.id = carCount + 1;
    printf("Enter brand: "); safeInput(c.brand, sizeof(c.brand));
    printf("Enter model: "); safeInput(c.model, sizeof(c.model));
    printf("Enter plate number: "); safeInput(c.plate, sizeof(c.plate));
    printf("Enter rent per day: "); scanf("%lf", &c.rentPerDay); clearInputBuffer();
    c.isAvailable = 1;
    strcpy(c.damageReport, "");
    cars[carCount++] = c;
    autoSave();
    printf("Car added successfully!\n");
    pausemsg();
}

void viewCars(int onlyAvailable) {
    printf("\n==== CAR LIST ====\n");
    for (int i = 0; i < carCount; i++) {
        if (onlyAvailable && !cars[i].isAvailable) continue;
        printf("ID:%d | %s %s | Plate:%s | Rent:PHP %.2f | %s",
            cars[i].id, cars[i].brand, cars[i].model, cars[i].plate,
            cars[i].rentPerDay, cars[i].isAvailable ? "Available" : "Rented");
        if (strlen(cars[i].damageReport) > 0)
            printf(" | Damage: %s", cars[i].damageReport);
        printf("\n");
    }
    pausemsg();
}

void searchCar() {
    char keyword[64];
    printf("Enter model or plate to search: ");
    safeInput(keyword, sizeof(keyword));
    for (int i = 0; i < carCount; i++) {
        if (strstr(cars[i].model, keyword) || strstr(cars[i].plate, keyword)) {
            printf("Found: ID:%d | %s %s | Plate:%s | Rent:PHP %.2f\n",
                cars[i].id, cars[i].brand, cars[i].model, cars[i].plate, cars[i].rentPerDay);
            pausemsg();
            return;
        }
    }
    printf("Car not found.\n");
    pausemsg();
}

void updateCar() {
    int id;
    printf("Enter Car ID to update: ");
    scanf("%d", &id); clearInputBuffer();
    for (int i = 0; i < carCount; i++) {
        if (cars[i].id == id) {
            printf("Update brand: "); safeInput(cars[i].brand, sizeof(cars[i].brand));
            printf("Update model: "); safeInput(cars[i].model, sizeof(cars[i].model));
            printf("Update plate: "); safeInput(cars[i].plate, sizeof(cars[i].plate));
            printf("Update rent per day: "); scanf("%lf", &cars[i].rentPerDay); clearInputBuffer();
            autoSave();
            printf("Car updated successfully!\n");
            pausemsg();
            return;
        }
    }
    printf("Car not found.\n");
    pausemsg();
}

void deleteCar() {
    int id;
    printf("Enter Car ID to delete: ");
    scanf("%d", &id); clearInputBuffer();
    for (int i = 0; i < carCount; i++) {
        if (cars[i].id == id) {
            for (int j = i; j < carCount - 1; j++)
                cars[j] = cars[j + 1];
            carCount--;
            autoSave();
            printf("Car deleted.\n");
            pausemsg();
            return;
        }
    }
    printf("Car not found.\n");
    pausemsg();
}

/* Report damaged car */
void reportDamagedCar() {
    int id;
    printf("Enter Car ID to report damage: ");
    scanf("%d", &id); clearInputBuffer();
    for (int i = 0; i < carCount; i++) {
        if (cars[i].id == id) {
            printf("Reporting damage for %s %s (Plate: %s)\n", cars[i].brand, cars[i].model, cars[i].plate);
            printf("Enter damage details (max 255 chars): ");
            safeInput(cars[i].damageReport, sizeof(cars[i].damageReport));
            autoSave();
            printf("Damage reported successfully.\n");
            pausemsg();
            return;
        }
    }
    printf("Car ID not found.\n");
    pausemsg();
}

/* Clear damage report */
void clearDamageReport() {
    int id;
    printf("Enter Car ID to clear damage report: ");
    scanf("%d", &id); clearInputBuffer();
    for (int i = 0; i < carCount; i++) {
        if (cars[i].id == id) {
            if (strlen(cars[i].damageReport) == 0) {
                printf("This car has no reported damage.\n");
                pausemsg();
                return;
            }
            strcpy(cars[i].damageReport, "");
            autoSave();
            printf("Damage report cleared. Car marked as repaired.\n");
            pausemsg();
            return;
        }
    }
    printf("Car not found.\n");
    pausemsg();
}

/* ===================== CUSTOMER MANAGEMENT ===================== */
void addCustomer() {
    Customer c;
    c.id = customerCount + 1;
    printf("Enter name: "); safeInput(c.name, sizeof(c.name));
    printf("Enter address: "); safeInput(c.address, sizeof(c.address));
    printf("Enter phone: "); safeInput(c.phone, sizeof(c.phone));
    printf("Enter license number: "); safeInput(c.license, sizeof(c.license));
    customers[customerCount++] = c;
    autoSave();
    printf("Customer added!\n");
    pausemsg();
}

void viewCustomers() {
    printf("\n==== CUSTOMER LIST ====\n");
    for (int i = 0; i < customerCount; i++) {
        printf("ID:%d | %s | %s | %s | %s\n", customers[i].id, customers[i].name, customers[i].address, customers[i].phone, customers[i].license);
    }
    pausemsg();
}

/* ===================== RENTAL MANAGEMENT ===================== */

void rentCar() {
    int carId, custId, days;
    printf("Enter Car ID: "); scanf("%d", &carId);
    printf("Enter Customer ID: "); scanf("%d", &custId);
    printf("Enter number of days: "); scanf("%d", &days);
    clearInputBuffer();

    for (int i = 0; i < carCount; i++) {
        if (cars[i].id == carId && cars[i].isAvailable) {
            Rental r;
            r.id = rentalCount + 1;
            r.carId = carId;
            r.customerId = custId;
            r.rentDays = days;
            r.totalCost = days * cars[i].rentPerDay;
            r.returned = 0;
            printf("Enter rent date: "); safeInput(r.rentDate, sizeof(r.rentDate));
            strcpy(r.returnDate, "N/A");
            r.lateFee = 0;
            rentals[rentalCount++] = r;
            cars[i].isAvailable = 0;
            autoSave();
            printf("Car rented successfully! Total cost: PHP %.2f\n", r.totalCost);
            pausemsg();
            return;
        }
    }
    printf("Car not available or not found.\n");
    pausemsg();
}

void returnCar() {
    int id;
    printf("Enter Rental ID to return: ");
    scanf("%d", &id); clearInputBuffer();
    for (int i = 0; i < rentalCount; i++) {
        if (rentals[i].id == id && rentals[i].returned == 0) {
            printf("Enter actual return date: "); safeInput(rentals[i].returnDate, sizeof(rentals[i].returnDate));
            int lateDays;
            printf("Enter number of late days (if any): "); scanf("%d", &lateDays); clearInputBuffer();
            rentals[i].lateFee = lateDays * 500; // ₱500 per late day
            rentals[i].totalCost += rentals[i].lateFee;
            rentals[i].returned = 1;

            // Mark car as available again
            for (int j = 0; j < carCount; j++) {
                if (cars[j].id == rentals[i].carId) {
                    cars[j].isAvailable = 1;
                    break;
                }
            }

            autoSave();
            printf("Car returned successfully! Total payment: PHP %.2f\n", rentals[i].totalCost);
            pausemsg();
            return;
        }
    }
    printf("Rental not found or already returned.\n");
    pausemsg();
}

void viewRentals() {
    printf("\n==== RENTAL RECORDS ====\n");
    for (int i = 0; i < rentalCount; i++) {
        printf("ID:%d | Car:%d | Cust:%d | Days:%d | Cost:%.2f | %s | Rent:%s | Return:%s\n",
            rentals[i].id, rentals[i].carId, rentals[i].customerId, rentals[i].rentDays,
            rentals[i].totalCost, rentals[i].returned ? "Returned" : "Active", rentals[i].rentDate, rentals[i].returnDate);
    }
    pausemsg();
}

/* ===================== FILE HANDLING ===================== */
void loadData() {
    FILE *f; char line[512];
    /* Load Cars */
    f = fopen(CAR_FILE, "r");
    if (f) {
        carCount = 0;
        while (fgets(line, sizeof(line), f)) {
            trimNewline(line);
            if (strlen(line) == 0) continue;
            Car c = {0};
            char *p = strtok(line, "|");
            if (!p) continue;
            c.id = atoi(p);
            p = strtok(NULL, "|"); strncpy(c.brand, p?p:"", sizeof(c.brand)-1);
            p = strtok(NULL, "|"); strncpy(c.model, p?p:"", sizeof(c.model)-1);
            p = strtok(NULL, "|"); strncpy(c.plate, p?p:"", sizeof(c.plate)-1);
            p = strtok(NULL, "|"); c.rentPerDay = p?atof(p):0.0;
            p = strtok(NULL, "|"); c.isAvailable = p?atoi(p):1;
            p = strtok(NULL, "|"); strncpy(c.damageReport, p?p:"", sizeof(c.damageReport)-1);
            cars[carCount++] = c;
        }
        fclose(f);
    }

    /* Load Customers */
    f = fopen(CUSTOMER_FILE, "r");
    if (f) {
        customerCount = 0;
        while (fgets(line, sizeof(line), f)) {
            trimNewline(line);
            if (strlen(line) == 0) continue;
            Customer c = {0};
            sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%[^|]", &c.id, c.name, c.address, c.phone, c.license);
            customers[customerCount++] = c;
        }
        fclose(f);
    }

    /* Load Rentals */
    f = fopen(RENTAL_FILE, "r");
    if (f) {
        rentalCount = 0;
        while (fgets(line, sizeof(line), f)) {
            trimNewline(line);
            if (strlen(line) == 0) continue;
            Rental r = {0};
            sscanf(line, "%d|%d|%d|%d|%lf|%d|%[^|]|%[^|]|%lf",
                &r.id, &r.carId, &r.customerId, &r.rentDays, &r.totalCost, &r.returned,
                r.rentDate, r.returnDate, &r.lateFee);
            rentals[rentalCount++] = r;
        }
        fclose(f);
    }
}

void saveData() {
    FILE *f;
    /* Save Cars */
    f = fopen(CAR_FILE, "w");
    if (f) {
        for (int i = 0; i < carCount; i++) {
            fprintf(f, "%d|%s|%s|%s|%.2f|%d|%s\n",
                cars[i].id, cars[i].brand, cars[i].model, cars[i].plate,
                cars[i].rentPerDay, cars[i].isAvailable,
                cars[i].damageReport[0] ? cars[i].damageReport : "");
        }
        fclose(f);
    }

    /* Save Customers */
    f = fopen(CUSTOMER_FILE, "w");
    if (f) {
        for (int i = 0; i < customerCount; i++) {
            fprintf(f, "%d|%s|%s|%s|%s\n",
                customers[i].id, customers[i].name, customers[i].address,
                customers[i].phone, customers[i].license);
        }
        fclose(f);
    }

    /* Save Rentals */
    f = fopen(RENTAL_FILE, "w");
    if (f) {
        for (int i = 0; i < rentalCount; i++) {
            fprintf(f, "%d|%d|%d|%d|%.2f|%d|%s|%s|%.2f\n",
                rentals[i].id, rentals[i].carId, rentals[i].customerId,
                rentals[i].rentDays, rentals[i].totalCost, rentals[i].returned,
                rentals[i].rentDate, rentals[i].returnDate, rentals[i].lateFee);
        }
        fclose(f);
    }
}

/* ===================== MENUS ===================== */
void carMenu() {
    int c;
    do {
        printf("\n==== CAR MANAGEMENT ====\n");
        printf("1. Add Car\n");
        printf("2. View All Cars\n");
        printf("3. View Available Cars\n");
        printf("4. Search Car\n");
        printf("5. Update Car\n");
        printf("6. Delete Car\n");
        printf("7. Report Damaged Car\n");
        printf("8. Clear Damage Report\n");
        printf("9. Back\n");
        printf("Choose: ");
        scanf("%d", &c); clearInputBuffer();
        switch (c) {
            case 1: addCar(); break;
            case 2: viewCars(0); break;
            case 3: viewCars(1); break;
            case 4: searchCar(); break;
            case 5: updateCar(); break;
            case 6: deleteCar(); break;
            case 7: reportDamagedCar(); break;
            case 8: clearDamageReport(); break;
        }
    } while (c != 9);
}

void customerMenu() {
    int c;
    do {
        printf("\n==== CUSTOMER MANAGEMENT ====\n");
        printf("1. Add Customer\n");
        printf("2. View Customers\n");
        printf("3. Back\n");
        printf("Choose: ");
        scanf("%d", &c); clearInputBuffer();
        switch (c) {
            case 1: addCustomer(); break;
            case 2: viewCustomers(); break;
        }
    } while (c != 3);
}

void rentalMenu() {
    int c;
    do {
        printf("\n==== RENTAL MANAGEMENT ====\n");
        printf("1. Rent Car\n");
        printf("2. Return Car\n");
        printf("3. View Rentals\n");
        printf("4. Back\n");
        printf("Choose: ");
        scanf("%d", &c); clearInputBuffer();
        switch (c) {
            case 1: rentCar(); break;
            case 2: returnCar(); break;
            case 3: viewRentals(); break;
        }
    } while (c != 4);
}

/* ===================== MAIN MENU ===================== */
int main() {
    loadData();
    int c;
    do {
        printf("\n==== CAR RENTAL MANAGEMENT SYSTEM ====\n");
        printf("1. Car Management\n");
        printf("2. Customer Management\n");
        printf("3. Rental Management\n");
        printf("4. Exit\n");
        printf("Choose: ");
        scanf("%d", &c); clearInputBuffer();
        switch (c) {
            case 1: carMenu(); break;
            case 2: customerMenu(); break;
            case 3: rentalMenu(); break;
        }
    } while (c != 4);
    saveData();
    printf("All data saved. Goodbye!\n");
    return 0;
}
