#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Animal {
protected:
    std::string name;
    int age;
    double weight;
    bool isYoung;
    bool isDead;

public:
    Animal(const std::string& name, int age, double weight)
        : name(name), age(age), weight(weight), isYoung(true), isDead(false) {}

    void ageAnimal(int days) {
        age += days;
        if (age >= 365 * 3) {
            isDead = true;
        }
    }

    void dieIfOld() {
        // Implement the logic if needed, or leave it empty for now
    }

    // Method to feed the animal, increasing its weight
    void feed() {
        weight += 0.5;
    }

    // Method to simulate the animal moving, decreasing its weight
    void move() {
        weight -= 0.25;
    }

    // Virtual method for producing a sound (specific to each animal type)
    virtual void sound() const = 0;

    // Virtual method to get information about the animal
    virtual std::string getInfo() const = 0;

    // Check if the animal is alive
    bool isAlive() const {
        return !isDead;
    }

    // Kill the animal (set status to dead)
    void kill() {
        isDead = true;
    }

    // Getter for the weight of the animal
    double getWeight() const {
        return weight;
    }
};

// Derived class for carnivores
class Carnivore : public Animal {
public:
    Carnivore(const std::string& name, int age, double weight) : Animal(name, age, weight) {}

    // Method for hunting, increasing weight
    void hunt() {
        weight += 1;
    }

    // Method for eating another animal, taking over half of its weight
    void eat(Animal* prey) {
        weight += prey->getWeight() / 2;
        prey->kill();
    }

    // New method to check if the animal dies due to age
    void dieIfOld() {
        if (age >= 365 * 3) {
            isDead = true;
        }
    }
};

// Derived class for herbivores
class Herbivore : public Animal {
protected:
    double lengthMoved;

public:
    Herbivore(const std::string& name, int age, double weight)
        : Animal(name, age, weight), lengthMoved(0) {}

    // Remove the 'override' specifier here
    void move() {
        lengthMoved += (rand() % 41 + 10) / 10.0; // random distance between 1.0 and 5.0
        weight -= 0.25;
    }

    // Method to check if the herbivore dies due to age
    void dieIfOld() {
        if (age >= 365 * 2) {
            isDead = true;
        }
    }
};

// Derived classes for specific animal types
class Elephant : public Herbivore {
public:
    Elephant(const std::string& name, int age, double weight) : Herbivore(name, age, weight) {}

    void sound() const override {
        std::cout << "Trumpet sound\n";
    }

    std::string getInfo() const override {
        return "Elephant: " + name + ", Age: " + std::to_string(age) + ", Weight: " + std::to_string(weight) +
               ", Status: " + (isYoung ? "Young" : "Older") + (isDead ? ", Status: Dead" : "");
    }
};

class Lion : public Carnivore {
public:
    Lion(const std::string& name, int age, double weight) : Carnivore(name, age, weight) {}

    void sound() const override {
        std::cout << "Roar\n";
    }

    std::string getInfo() const override {
        return "Lion: " + name + ", Age: " + std::to_string(age) + ", Weight: " + std::to_string(weight) +
               ", Status: " + (isYoung ? "Young" : "Older") + (isDead ? ", Status: Dead" : "");
    }
};

class Monkey : public Herbivore {
public:
    Monkey(const std::string& name, int age, double weight) : Herbivore(name, age, weight) {}

    void sound() const override {
        std::cout << "Chatter sound\n";
    }

    std::string getInfo() const override {
        return "Monkey: " + name + ", Age: " + std::to_string(age) + ", Weight: " + std::to_string(weight) +
               ", Status: " + (isYoung ? "Young" : "Older") + (isDead ? ", Status: Dead" : "");
    }
};

class Dolphin : public Carnivore {
public:
    Dolphin(const std::string& name, int age, double weight) : Carnivore(name, age, weight) {}

    // Removing the override keyword for hunt and feed
    void hunt() {}
    
    // Removing the override keyword for feed
    void feed() {
        weight += 1.5;
    }

    // Implementing the sound method to satisfy the pure virtual function in Animal
    void sound() const override {
        std::cout << "Clicking sound\n";
    }

    // Adding dieIfOld method
    void dieIfOld() {
        // Dolphins never die by age
    }

    std::string getInfo() const override {
        return "Dolphin: " + name + ", Age: " + std::to_string(age) + ", Weight: " + std::to_string(weight) +
               ", Status: " + (isYoung ? "Young" : "Older") + (isDead ? ", Status: Dead" : "");
    }
};

// Zoo class to manage a collection of animals
class Zoo {
private:
    std::vector<Animal*> animals;

public:
    // Method to add an animal to the zoo
    void addAnimal(const std::string& type, const std::string& name, int age, double weight) {
        Animal* newAnimal = nullptr;

        // Create an instance of the appropriate animal type based on the input
        if (type == "Elephant") {
            newAnimal = new Elephant(name, age, weight);
        } else if (type == "Lion") {
            newAnimal = new Lion(name, age, weight);
        } else if (type == "Monkey") {
            newAnimal = new Monkey(name, age, weight);
        } else if (type == "Dolphin") {
            newAnimal = new Dolphin(name, age, weight);
        }

        // Add the new animal to the zoo
        if (newAnimal != nullptr) {
            animals.push_back(newAnimal);
        }
    }

    // Method to display information about all animals in the zoo
    void showZoo() const {
        for (const auto& animal : animals) {
            std::cout << animal->getInfo() << std::endl;
        }
    }

    // Method to remove dead animals from the zoo
    void cleanUpZoo() {
        auto it = std::remove_if(animals.begin(), animals.end(),
                                 [](const Animal* animal) { return !animal->isAlive(); });

        animals.erase(it, animals.end());
    }

    // Method to feed all animals in the zoo
    void feed() {
        for (auto& animal : animals) {
            animal->feed();
        }
    }

    // Method for one animal to eat another randomly selected animal
    void eat() {
        if (animals.size() < 2) {
            std::cout << "Not enough animals to eat!\n";
            return;
        }

        srand(static_cast<unsigned>(time(nullptr)));
        int predatorIndex = rand() % animals.size();
        int preyIndex = predatorIndex;
        while (preyIndex == predatorIndex) {
            preyIndex = rand() % animals.size();
        }

        Carnivore* predator = dynamic_cast<Carnivore*>(animals[predatorIndex]);
        if (predator != nullptr) {
            predator->eat(animals[preyIndex]);
        }
    }

// In the Zoo class
void waitDays(int days) {
    for (int i = 0; i < days; ++i) {
        for (auto& animal : animals) {
            animal->ageAnimal(1);

            // Herbivores move each day
            if (dynamic_cast<Herbivore*>(animal) != nullptr) {
                animal->move();
                dynamic_cast<Herbivore*>(animal)->dieIfOld();  // Call dieIfOld for Herbivores
            }

            // Carnivores hunt, check for death by age
            if (dynamic_cast<Carnivore*>(animal) != nullptr) {
                Carnivore* carnivore = dynamic_cast<Carnivore*>(animal);
                carnivore->hunt();
                carnivore->dieIfOld();  // Call dieIfOld for Carnivores
            }

            // Call dieIfOld for all animals
            animal->dieIfOld();
        }
    }
}  // Ensure this closing brace is present

};  // Ensure this closing brace is present for the Zoo class

// Test function
void testZoo() {
    Zoo zoo;

    // Adding animals to the zoo
    zoo.addAnimal("Elephant", "Dumbo", 50, 300);
    zoo.addAnimal("Lion", "Simba", 30, 150);
    zoo.addAnimal("Monkey", "Bubbles", 20, 50);
    zoo.addAnimal("Dolphin", "Flipper", 10, 200);

    // Displaying initial zoo information
    zoo.showZoo();

    // Feeding the animals
    zoo.feed();

    // Simulating one animal eating another
    zoo.eat();

    // Displaying zoo information after feeding and eating
    zoo.showZoo();

    // Simulating the passage of one year
    zoo.waitDays(365);

    // Displaying zoo information after one year
    zoo.showZoo();

    // Cleaning up the zoo by removing dead animals
    zoo.cleanUpZoo();

    // Displaying final zoo information
    zoo.showZoo();
}

// Main function
int main() {
    // Calling the test function
    testZoo();

    return 0;
}
