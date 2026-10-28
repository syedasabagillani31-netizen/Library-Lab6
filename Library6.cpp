
#include <iostream>
#include <string>
#include <utility>
#include <limits>
using namespace std;

// ========================================
// Book Node
// ========================================
struct BookNode
{
    int bookID;
    string title;
    BookNode* next;
};

// ========================================
// Circular Linked List Class
// head points to the last node
// head->next points to the first node
// ========================================
class CircularLinkedList
{
private:
    BookNode* head;
    BookNode* currentBook;
    int count;

    // Get node at an index
    BookNode* GetNodeAt(int index)
    {
        if (head == nullptr || index < 0 || index >= count)
            return nullptr;

        BookNode* current = head->next;

        for (int i = 0; i < index; i++)
            current = current->next;

        return current;
    }

    // Normalize index into the circular list
    int NormalizeIndex(int index)
    {
        return ((index % count) + count) % count;
    }

    // Swap book data
    void SwapData(BookNode* a, BookNode* b)
    {
        swap(a->bookID, b->bookID);
        swap(a->title, b->title);
    }

    // Reset rotation to the first book
    void ResetCurrent()
    {
        currentBook = (head == nullptr) ? nullptr : head->next;
    }

public:

    // Constructor
    CircularLinkedList()
    {
        head = nullptr;
        currentBook = nullptr;
        count = 0;
    }

    // Check collection status
    bool IsEmpty()
    {
        return count == 0;
    }

    bool IsNotEmpty()
    {
        return count > 0;
    }

    // Insert at head (beginning)
    void Insert(BookNode* book)
    {
        if (book == nullptr)
            return;

        if (head == nullptr)
        {
            head = book;
            book->next = book;
        }
        else
        {
            book->next = head->next;
            head->next = book;
        }

        count++;
        ResetCurrent();
    }

    // Remove from head (beginning)
    BookNode* Remove()
    {
        if (IsEmpty())
            return nullptr;

        BookNode* first = head->next;

        if (first == head)
        {
            head = nullptr;
        }
        else
        {
            head->next = first->next;
        }

        first->next = nullptr;
        count--;
        ResetCurrent();

        return first;
    }

    // Insert at a specified index
    void InsertAt(BookNode* book, int index)
    {
        if (book == nullptr)
            return;

        if (count == 0)
        {
            head = book;
            book->next = book;
            count++;
            ResetCurrent();
            return;
        }

        if (index < 0)
            index = NormalizeIndex(index);

        if (index > count)
            index %= count;

        // Insert at beginning
        if (index == 0)
        {
            Insert(book);
            return;
        }

        // Insert at end
        if (index == count)
        {
            book->next = head->next;
            head->next = book;
            head = book;
            count++;
            ResetCurrent();
            return;
        }

        // Insert in middle
        BookNode* previous = GetNodeAt(index - 1);

        book->next = previous->next;
        previous->next = book;

        count++;
        ResetCurrent();
    }

    // Remove at a specified index
    BookNode* RemoveAt(int index)
    {
        if (IsEmpty())
            return nullptr;

        index = NormalizeIndex(index);

        if (index == 0)
            return Remove();

        BookNode* previous = GetNodeAt(index - 1);
        BookNode* target = previous->next;

        previous->next = target->next;

        if (target == head)
            head = previous;

        target->next = nullptr;
        count--;
        ResetCurrent();

        return target;
    }

    // Swap books at two indexes
    void Swap(int index1, int index2)
    {
        if (IsEmpty())
            return;

        index1 = NormalizeIndex(index1);
        index2 = NormalizeIndex(index2);

        if (index1 == index2)
            return;

        BookNode* first = GetNodeAt(index1);
        BookNode* second = GetNodeAt(index2);

        SwapData(first, second);
        ResetCurrent();
    }

    // Sort books by ID
    void Sort()
    {
        if (count < 2)
            return;

        BookNode* current = head->next;

        for (int i = 0; i < count - 1; i++)
        {
            BookNode* nextNode = current->next;

            for (int j = i + 1; j < count; j++)
            {
                if (current->bookID > nextNode->bookID)
                    SwapData(current, nextNode);

                nextNode = nextNode->next;
            }

            current = current->next;
        }

        ResetCurrent();
    }

    // Reverse the circular linked list
    void Reverse()
    {
        if (count < 2)
            return;

        BookNode* oldTail = head;
        BookNode* oldFirst = head->next;

        BookNode* previous = oldTail;
        BookNode* current = oldFirst;

        while (current != oldTail)
        {
            BookNode* nextNode = current->next;

            current->next = previous;
            previous = current;
            current = nextNode;
        }

        oldTail->next = previous;
        head = oldFirst;

        ResetCurrent();
    }

    // Display all books
    void Print()
    {
        if (IsEmpty())
        {
            cout << "\nNo books in the collection.\n";
            return;
        }

        BookNode* current = head->next;

        cout << "\nIndex\tBook ID\tTitle\n";
        cout << "----------------------------------------\n";

        for (int i = 0; i < count; i++)
        {
            cout << i << "\t"
                 << current->bookID << "\t"
                 << current->title << endl;

            current = current->next;
        }

        cout << "Total books: " << count << endl;
        cout << "Last book connects back to the first book.\n";
    }

    // Move to the next book in circular order
    void MoveToNextBook()
    {
        if (IsEmpty())
        {
            cout << "\nNo books available.\n";
            return;
        }

        cout << "\nCurrent Book: "
             << currentBook->title << endl;

        currentBook = currentBook->next;

        cout << "Next Book: "
             << currentBook->title << endl;
    }

    // Copy constructor
    CircularLinkedList(const CircularLinkedList& other)
    {
        head = nullptr;
        currentBook = nullptr;
        count = 0;

        BookNode* current = other.head == nullptr
                            ? nullptr : other.head->next;

        for (int i = 0; i < other.count; i++)
        {
            BookNode* copy = new BookNode{
                current->bookID,
                current->title,
                nullptr
            };

            InsertAt(copy, count);
            current = current->next;
        }
    }

    // Assignment operator
    CircularLinkedList& operator=(const CircularLinkedList& other)
    {
        if (this == &other)
            return *this;

        while (IsNotEmpty())
            delete Remove();

        BookNode* current = other.head == nullptr
                            ? nullptr : other.head->next;

        for (int i = 0; i < other.count; i++)
        {
            BookNode* copy = new BookNode{
                current->bookID,
                current->title,
                nullptr
            };

            InsertAt(copy, count);
            current = current->next;
        }

        return *this;
    }

    // Destructor
    ~CircularLinkedList()
    {
        while (IsNotEmpty())
            delete Remove();
    }
};

// ========================================
// Create a Book Node
// ========================================
BookNode* CreateBook(int id, string title)
{
    BookNode* book = new BookNode;

    book->bookID = id;
    book->title = title;
    book->next = nullptr;

    return book;
}

// ========================================
// Read integer safely
// ========================================
int ReadInt(string message)
{
    int value;

    while (true)
    {
        cout << message;

        if (cin >> value)
            return value;

        cout << "Invalid input. Please enter a number.\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ========================================
// Main Menu
// ========================================
int main()
{
    CircularLinkedList library;

    // Initial books
    library.InsertAt(CreateBook(103, "Operating Systems"), 0);
    library.InsertAt(CreateBook(101, "Data Structures"), 1);
    library.InsertAt(CreateBook(105, "Computer Networks"), 2);
    library.InsertAt(CreateBook(102, "Database Systems"), 3);
    library.InsertAt(CreateBook(104, "Software Engineering"), 4);

    int choice;

    do
    {
        cout << "\n========================================\n";
        cout << " CIRCULAR LIBRARY BOOK DISPLAY SYSTEM\n";
        cout << "========================================\n";
        cout << "1. Display All Books\n";
        cout << "2. Insert Book at Head\n";
        cout << "3. Remove Book from Head\n";
        cout << "4. Insert Book at Index\n";
        cout << "5. Remove Book at Index\n";
        cout << "6. Swap Books\n";
        cout << "7. Sort Books by ID\n";
        cout << "8. Reverse Book Collection\n";
        cout << "9. Move to Next Book\n";
        cout << "10. Check Collection Status\n";
        cout << "11. Exit\n";
        cout << "========================================\n";

        choice = ReadInt("Enter your choice: ");

        switch (choice)
        {
        case 1:
            library.Print();
            break;

        case 2:
        {
            int id = ReadInt("Enter book ID: ");

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string title;
            cout << "Enter book title: ";
            getline(cin, title);

            library.Insert(CreateBook(id, title));

            cout << "Book inserted at head successfully.\n";
            break;
        }

        case 3:
        {
            BookNode* removed = library.Remove();

            if (removed != nullptr)
            {
                cout << "Removed book: "
                     << removed->title << endl;
                delete removed;
            }
            else
            {
                cout << "Collection is empty.\n";
            }

            break;
        }

        case 4:
        {
            int id = ReadInt("Enter book ID: ");

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            string title;
            cout << "Enter book title: ";
            getline(cin, title);

            int index = ReadInt("Enter index: ");

            library.InsertAt(CreateBook(id, title), index);

            cout << "Book inserted successfully.\n";
            break;
        }

        case 5:
        {
            int index = ReadInt("Enter index to remove: ");

            BookNode* removed = library.RemoveAt(index);

            if (removed != nullptr)
            {
                cout << "Removed book: "
                     << removed->title << endl;
                delete removed;
            }
            else
            {
                cout << "Collection is empty.\n";
            }

            break;
        }

        case 6:
        {
            int index1 = ReadInt("Enter first index: ");
            int index2 = ReadInt("Enter second index: ");

            library.Swap(index1, index2);

            cout << "Swap operation completed.\n";
            break;
        }

        case 7:
            library.Sort();
            cout << "Books sorted by ID.\n";
            library.Print();
            break;

        case 8:
            library.Reverse();
            cout << "Book collection reversed.\n";
            library.Print();
            break;

        case 9:
            library.MoveToNextBook();
            break;

        case 10:
            if (library.IsEmpty())
                cout << "The collection is empty.\n";
            else
                cout << "The collection contains "
                     << "books and is not empty.\n";
            break;

        case 11:
            cout << "Exiting Circular Library System.\n";
            break;

        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 11);

    return 0;
}
