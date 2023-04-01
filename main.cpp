// Program Name: Library_Simulation.cpp

// Programmer_1 Name: Catherine Xu
// Programmer_1 ID: 1869259
// Programmer_2 Name: Elise Zur
// Programmer_2 ID: 1851550
// Programmer_3 Name: Maddie Ananda Subramanian
// Programmer_3 ID: 1662198
// Programmer_4 Name: Visitxay Hanmonty
// Programmer_4 ID: 1834966
// Programmer_5 Name: Ayra Jafri
// Programmer_5 ID: 1895238

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <map>
using namespace std;

#include <cstdlib>
#include <cmath>

#include "DynamicArray.h"
#include "HashTable.h"

struct book
{
  string title;
  string author;
  string ISBN;
  string genre;
  
  int totalRating; ///sum of all rating
  int numRating; //num of people rated the book
  //actual rating = (double)totalRating / numRating
  
  bool avaliable;
};

struct user
{
  unsigned int ID;
  string password;
  string bookBorrowed; //ISBN
  int borrow;
};

//prototype
int login(bool& run, DynamicArray<user>& users, HashTable<unsigned int, int, 100000>& userID, int& userSize);

int signup(DynamicArray<user>& users, HashTable<unsigned int, int, 100000>& userID, int& userSize);

void getBook(ifstream& bookI, DynamicArray<book>& books, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookSize); //get book list from file

void getUser(ifstream& userI, DynamicArray<user>& users, HashTable<unsigned int, int, 100000>& userID, int& userSize);

void searchBook(int currentUser, int& bookIndex, DynamicArray<book>& books, DynamicArray<user>& users, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, bool& run, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookSize, string& userISBN);

void returnBook(int& bookIndex, DynamicArray<book>& books, DynamicArray<user>& users, int currentUser, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, bool& run, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookSize, string& userISBN);

void getRecommendation(DynamicArray<user>& users, int currentUser, DynamicArray<book>& books, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookIndex, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, bool& run, int& bookSize, string& ISBN);

void requestBook(DynamicArray<book>& books, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, int& bookSize, string& userISBN);

void checkOut(DynamicArray<book>& books, DynamicArray<user>& users, int currentUser, int& bookIndex, HashTable<string, int, 75000>& bookISBN, string& userISBN);

int hashCodeS(const string& key);

int hashCodeI(const unsigned int& key);

void mainMenu(int currentUser, int bookIndex, DynamicArray<book>& books, DynamicArray<user>& users, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, bool& run, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookSize, string& userISBN);

int main()
 {
  // declarations
  string buf;
  string buf2;
  string userISBN;
  int option;
  int currentUser; //index of current user in the array
  int userSize = 0; //the size of the user array
  int bookSize = 0;
  int ogUserSize, ogBookSize;
  int bookIndex = 0;
  ifstream userI;
  ofstream userO;
  ifstream bookI;
  ofstream bookO;
  DynamicArray<user> users; //all users
  DynamicArray<book> books; //all books in library
  HashTable<string, int, 75000> bookISBN(hashCodeS); //key: ISBN, value: index stored in books array
  HashTable<string, int, 75000> bookTitle(hashCodeS); //key: title, value: index stored in books array
  HashTable<unsigned int, int, 100000> userID(hashCodeI); //key: user ID, value: index stored in users array
  HashTable<string, map<double, int>, 75000> bookGenre(hashCodeS); //key: title, value: index stored in books array
  bool run = true;
  bool loggedIn = true;
  
  // welcome page
  cout << "**************************************************" << endl;
  cout << "*                                                *" << endl;
  cout << "*            Welcome to the library!             *" << endl;
  cout << "*                                                *" << endl;
  cout << "**************************************************" << endl << endl;
  
  // open file
  userI.open("Library_Users.txt");
  bookI.open("Library_Books.txt");

  // check if open
  if(!userI.is_open() || !bookI.is_open())
  {
    cout << "Error. Unable to open the file." << endl;
    return 1;
  }
  
  // copy book and user info to array and hashtable
  getBook(bookI, books, bookISBN, bookTitle, bookGenre, bookSize);
  getUser(userI, users, userID, userSize);
  // getGenre();
  ogBookSize = bookSize;
  ogUserSize = userSize;
  

  while(run)
  {
    cout << "User Menu:\n" << "----------\n";
    cout << "[1] Log in\n" << "[2] Create a new account\n" << "[Q] Quit the system\n";
    cout << "\n\tEnter: ";
    cin >> buf;
    
    // if quit
    if(buf == "Q" || buf == "q") break;
    
    // not quit
    option = atoi(buf.c_str());
    
    // invalid input
    while(option != 1 && option != 2)
    {
      cout << "Invalid input, please enter again: ";
      cin >> buf;
      
      if(buf == "Q" || buf == "q"){
        run = false;
        option = -1;
        break;
      }
      option = atoi(buf.c_str());
    }
    
    // log in
    if(option == 1)
    {
      currentUser = login(run, users, userID, userSize);
    }
    // create new account and log in
    else if(option == 2)
    {
      currentUser = signup(users, userID, userSize);
    }
    else continue;
  
    if(currentUser == -1) continue;

    loggedIn = false;
    
    // main menu
    mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);

  }
  
  //update files
  //users
   userO.open("Library_Users.txt");
   for(int i = 0; i < userSize; i++)
   {
     if(i != 0){
       userO << '\n';
     }
     userO << users[i].ID << '\n' << users[i].password << '\n' << users[i].bookBorrowed << '\n' << users[i].borrow;
   }
   //books
   bookO.open("Library_Books.txt");
   for(int i = 0; i < bookSize; i++)
   {
     if(i != 0){
       bookO << '\n';
     }
     bookO << books[i].title << '\n' << books[i].author << '\n' << books[i].ISBN << '\n' << books[i].genre << '\n' << books[i].totalRating << '\n' << books[i].numRating << '\n' << books[i].avaliable;
   }
  
  //close file
  userI.close();
  userO.close();
  bookI.close();
  bookO.close();
  
  return 0;
}

/**
 * log in for existing user. Ask the user to enter ID and password and compare to list of existing users
 * the user object will be returned if the user is found, if not, the user can either try again, sign up, or quit the program.
 *
 * @param run  if continue run the program
 * @param users  the list of all users, will be user to check password
 * @param userID  it will be use to see if user ID exist, then we could use the value, which is the index where the user is stored in the array to check the password
 * @param userSize the size of the user array, may be passed to signup function
 *
 * @return the index of the user in the array
 */
int login(bool& run, DynamicArray<user>& users, HashTable<unsigned int, int, 100000>& userID, int& userSize)
{
  //fields
  int currentUser;
  unsigned int ID;
  string password;
  string buf;
  int temp;

  //get ID
  cout << "Please enter the ID: ";
  cin >> buf;
  ID = atoi(buf.c_str());

  //get password
  cout << "Please enter the password: ";
  cin >> password;

  //compare
  while(true)
  {
    if(userID.containsKey(ID) && users[userID[ID]].password == password)
    {
      currentUser = userID[ID];
      break;
    }
    //if not exist
    else
    {
      //print message
      cout << "Sorry, the ID/password is incorrect." << endl;
      //try again/sign up/quit
      cout << "Please enter [0] to quit, [1] to try again, or [2] to sign up: ";
      cin >> buf;
      temp = atoi(buf.c_str());

      if(temp == 0)
      {
        run = false;
        return -1;
      }
      //try again
      else if(temp == 1)
      {
        //get ID
        cout << "Please enter the ID: ";
        cin >> buf;
        ID = atoi(buf.c_str());

        //get password
        cout << "Please enter the password: ";
        cin >> password;

        continue;
      }
      //sign up
      else if(temp == 2)
      {
        currentUser = signup(users, userID, userSize);
        return currentUser;
      }
    }
  }
  return currentUser;
}

/**
 * generate user ID, set password, store user into list
 *
 * @param users  where new user will be added
 * @param userID  this will also be updated once a new user is added
 * @param userSize the size of the user array
 *
 * @return the index of the new user in the array, which will be the last index
 */
int signup(DynamicArray<user>& users, HashTable<unsigned int, int, 100000>& userID, int& userSize)
{
  //fields
  user current;
  string password;
  
  //assign ID
  int ID = 1000 + userSize;
  
  //create password
  cin.ignore();
  cout << "Please create a password: ";
  getline(cin, password);
  
  //store into users
  current.ID = ID;
  current.password = password;
  current.bookBorrowed = "N/A";
  current.borrow = 0;
  users[userSize] = current;
  userID[ID] = userSize;
  userSize++;
  
  cout << "Your new ID is: " << ID << endl;
  
  return userSize - 1;
}

/**
 * copy book info into the array
 *
 * @param bookI  this will be used to get books from the file
 * @param books  this is the array where the book info will be stored
 * @param bookISBN  we will store the ISBN as a key and the index of where the book info is stored in the books array as the value
 * @param bookTitle we will store the book title as a key and the index of where the book info is stored in the books array as the value
 * @param bookSize the size of the book array. Increment each time a book info is added
 */
void getBook(ifstream& bookI, DynamicArray<book>& books, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookSize)
{
  //fields
  book temp;
  string buf;
  
  while(bookI.good())
  {
    //get info
    getline(bookI, temp.title);
    getline(bookI, temp.author);
    getline(bookI, temp.ISBN);
    getline(bookI, temp.genre);
    getline(bookI, buf);
    temp.totalRating = atoi(buf.c_str());
    getline(bookI, buf);
    temp.numRating = atoi(buf.c_str());
    getline(bookI, buf);
    temp.avaliable = atoi(buf.c_str());
    
    //store info
    books[bookSize] = temp;
    bookISBN[temp.ISBN] = bookSize;
    bookTitle[temp.title] = bookSize;
    bookGenre[temp.genre][(double)temp.totalRating/temp.numRating] = bookSize;
    bookSize++;
  }
}

/**
 * copy user info into the array
 *
 * @param userI  this will be used to get users from the file
 * @param users  this is the array where the user info will be stored
 * @param userID  we will store the user ID as a key and the index of where the user info is stored in the users array as the value
 * @param userSize the size of the user array. Increment each time a user info is added
 */
void getUser(ifstream& userI, DynamicArray<user>& users, HashTable<unsigned int, int, 100000>& userID, int& userSize)
{
  //fields
  user temp;
  string buf;
  
  while(userI.good())
  {
    //get info
    getline(userI, buf);
    temp.ID = atoi(buf.c_str());
    getline(userI, temp.password);
    getline(userI, temp.bookBorrowed);
    getline(userI, buf);
    temp.borrow = atoi(buf.c_str());
    
    //store info
    users[userSize] = temp;
    userID[temp.ID] = userSize;
    userSize++;
  }
}

/**
 * search for book, books can be added if not found by calling requestBook function. If the searched book if found, the user can check out the book by calling the checkOut function
 *
 * @param books  this will be used to print out book info if found
 * @param bookISBN  this will be used to search for book using ISBN
 * @param bookTitle  this will be used to search for book using book titles
 * @param bookSize the size of the book array. May be passed to request book
 */
void searchBook(int currentUser, int& bookIndex, DynamicArray<book>& books, DynamicArray<user>& users, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, bool& run, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookSize, string& userISBN)
{
  string userSearch;
  string userBookTitle;
  string userIndex;
  bool bookFound = false;
  bool available = false;

  cin.ignore();
  cout << "\n -------------------\n";
  cout << "| Search For a Book |\n";
  cout << " -------------------\n";
  cout << "*      Enter [1] Search Books By ISBN                          *" << endl;
  cout << "*            [2] Search Books By Title                         *" << endl;
  cout << "          > ";
  getline(cin, userSearch);
  
  switch(atoi(userSearch.c_str()))
  {

    case 1:
        cout << "Enter your book ISBN key here: ";
        getline(cin, userISBN);

        if(bookISBN.containsKey(userISBN))
        {
          bookFound = true;
          bookIndex = bookISBN[userISBN];
          cout << "\nWe found your book!\n";
          cout << "Book Information:\n";

          // book info
          cout << "    Title: " << books[bookISBN[userISBN]].title << endl;
          cout << "    Author: " << books[bookISBN[userISBN]].author << endl;
          cout << "    ISBN: " << userISBN << endl;
          cout << "    Rating: " << (double)books[bookISBN[userISBN]].totalRating/books[bookISBN[userISBN]].numRating << endl;
          cout << "    Availability: ";
          if(books[bookISBN[userISBN]].avaliable)
          {
            available = true;
            cout << "available" << endl << endl;
          }
          else{
            available = false;
            cout << "unavailable" << endl << endl;
          }

        }
        else
        {
          bookFound = false;
          cout << "I'm sorry, we could not find the book matching the ISBN " << ".\n";

          cout << "\n*      Would you like to request a new book?          *\n";
          cout << "       Enter [Y] or [y] to request the book\n";
          cout << "             [1] to return to the main menu\n";
          cout << "             [Q] or [q] to quit\n";
          cout << "       > ";
          
          cin >> userIndex;
          if(userIndex == "Y" || userIndex == "y")
          {
            requestBook(books, bookISBN, bookTitle, bookSize, userISBN);

            cout << "\n       Enter [1] to return to the main menu\n";
            cout << "             [Q] or [q] to quit\n";
            cin >> userIndex;

            if(userIndex == "Q" || userIndex == "q")
            {
              return;
            }
            else if (userIndex == "1")
            {
              mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
            }
          }
          else if(userIndex == "Q" || userIndex == "q")
          {
            return;
          }
          else if (userIndex == "1")
          {
            mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
          }

        }
            break;

    case 2:
        cout << "Enter your title here: ";
        getline(cin, userBookTitle);
        if(bookTitle.containsKey(userBookTitle))
        {
          bookFound = true;
          bookIndex = bookTitle[userBookTitle];
          cout << "We found your book!\n";
          cout << "Book Information:\n";

         //book info
          cout << "    Title: " << userBookTitle << endl;
          cout << "    Author: " << books[bookTitle[userBookTitle]].author << endl;
          cout << "    ISBN: " << books[bookTitle[userBookTitle]].ISBN << endl;
          cout << "    Rating: " << (double)books[bookTitle[userBookTitle]].totalRating/books[bookTitle[userBookTitle]].numRating << endl;
          cout << "    Availability: ";
          if(books[bookTitle[userBookTitle]].avaliable)
          {
            available = true;
            cout << "available" << endl << endl;
          }
          else
          {
            available = false;
            cout << "unavailable" << endl << endl;
          }

        }
        else
        {
          bookFound = false;
          cout << "Sorry, we weren't able to find " << userBookTitle << endl;

          cout << "\n*      Would you like to request a new book?          *\n";
          cout << "       Enter [Y] or [y] to request the book\n";
          cout << "             [1] to return to the main menu\n";
          cout << "             [Q] or [q] to quit\n";
          cout << "       > ";
          
          cin >> userIndex;
          if(userIndex == "Y" || userIndex == "y")
          {
            requestBook(books, bookISBN, bookTitle, bookSize, userISBN);
            
            cout << "\n       Enter [1] to return to the main menu\n";
            cout << "             [Q] or [q] to quit\n";
            cin >> userIndex;

            if(userIndex == "Q" || userIndex == "q")
            {
              return;
            }
            else if (userIndex == "1")
            {
              mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
            }
          }
          else if(userIndex == "Q" || userIndex == "q")
          {
            return;
          }
          else if (userIndex == "1")
          {
            mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
          }
        }

            break;
    default:
      cout << "Error please enter [1] or [2]\n";
  }

  // when a book has been found for both case 1 & 2
  // if statements are checking to see if the book
  // is available to be checked out
  if (available)
  {
    //borrow
    cout << "\n*      Would you like to borrow the book?          *\n";
    cout << "       Enter [Y] or [y] to borrow the book\n";
    cout << "             [1] to return to the main menu\n";
    cout << "             [Q] or [q] to quit\n";
    cout << "       > ";

    cin >> userIndex;
    if(userIndex == "Y" || userIndex == "y")
    {
      checkOut(books, users, currentUser, bookIndex, bookISBN, userISBN);

      cout << "\n       Enter [1] to return to the main menu\n";
      cout << "             [Q] or [q] to quit\n";
      cin >> userIndex;

      if(userIndex == "Q" || userIndex == "q")
      {
        return;
      }
      else if (userIndex == "1")
      {
        mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
      }
    }
    else if(userIndex == "Q" || userIndex == "q")
    {
      return;
    }
    else if (userIndex == "1")
    {
      mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
    }
  }
}


/**
 * return a book. a fine will be calculated if return after 30 days. ask the user for rating of the book
 *
 * @param books  the avalibity and rating of the book will be updated
 * @param users  it will be used with the currentUser to check if the book is returned in time and user info will be updated once the book is returned
 * @param currentUser  this is the index of where the user is stored in the users array
 * @param bookISBN key: ISBN, value: index where the book store in the books array
 * @param bookSize the size of the book array. May be passed to main menu
 */
void returnBook(int& bookIndex, DynamicArray<book>& books, DynamicArray<user>& users, int currentUser, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, bool& run, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookSize, string& userISBN)
{
  string buf2;
  if(users[currentUser].bookBorrowed == "N/A")
  {
    cout << "You don't have any book to return." << endl;

    cout << "\nPress [1] to Return to menu or enter [Q] or [q] to quit: ";
    cin >> buf2;
  
    if(buf2 == "Q" || buf2 == "q")
    {
      return;
    }
    else if (buf2 == "1")
    {
      mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
    }
  }
  
  // declarations
  string buf;
  int rating;
  bookIndex = bookISBN[users[currentUser].bookBorrowed];

  // get time
  time_t t = time(0);
  struct tm * timeStruct = localtime(&t);
  int time = timeStruct->tm_yday + 356 * (timeStruct->tm_year);
  
  // update
  books[bookIndex].avaliable = 1;
  users[currentUser].bookBorrowed = "N/A";
  
  // if the book expired
  if((time - users[currentUser].borrow) > 30)
  {
    cout << "Your book has expired for " << time - users[currentUser].borrow - 30 << " days." << endl;
    cout << "Please pay your fine of " << 0.1 * (time - users[currentUser].borrow - 30) << " dollars at the cashier." << endl;
  }
  
  users[currentUser].borrow = 0;

  // ask for rating
  cout << "Please rate " << books[bookIndex].title << "on the scale of 0 - 5 (only whole numbers) or enter [Q] to quit: ";
  cin >> buf;
  
  if(buf == "Q" || buf == "q") return;
  
  // check rating
  rating = atoi(buf.c_str());
  while(rating < 0 || rating > 5)
  {
    cout << "Invalid input. Please enter again: ";
    cin >> buf;
    
    if(buf == "Q" || buf == "q") return;
    
    rating = atoi(buf.c_str());
  }
  
  books[bookIndex].totalRating += rating;
  books[bookIndex].numRating++;
  
  // return or quit
  cout << "Thank you for your time! Please visit us again.\n";
  cout << "Press [1] to Return to menu or enter [Q] or [q] to quit: ";
  cin >> buf2;
  
  if(buf2 == "Q" || buf2 == "q") return;
  else if (buf2 == "1")
    mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
}

void getRecommendation(DynamicArray<user>& users, int currentUser, DynamicArray<book>& books, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookIndex, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, bool& run, int& bookSize, string& userISBN)
{
  string userGenre;
  string buf;
  srand((unsigned int)time(NULL));
  int random = rand() % 10;

  cout << "\n ---------------------\n";
  cout << "| Book Reccomendations |\n";
  cout << " ---------------------\n";
  cout << "*      Enter [1] Fiction Books                          *" << endl;
  cout << "*            [2] Thriller Books                         *" << endl;
  cout << "*            [3] Romance Books                          *" << endl;
  cout << "*            [4] Nonfiction Books                       *" << endl;
  cout << "          > ";

  cin >> userGenre;

  switch(atoi(userGenre.c_str()))
  {
    case 1:
    {
      // contains key func that looks for genre
      userGenre = "Fiction";
      if(bookGenre.containsKey(userGenre))
      {
        cout << "We found a book for you!\n";

        auto it = bookGenre[userGenre].rbegin();
        for(int i = 0; i < random && i < bookGenre[userGenre].size(); i++){
          it--;
        }
        cout << "Title: " << books[it->second].title << '\n';
        cout << "Genre: " << userGenre << '\n' << '\n';
      }
      break;
    }
    
    case 2:
    {
      // contains key func that looks for genre
      userGenre = "Thriller";
      if(bookGenre.containsKey(userGenre))
      {
        cout << "We found a book for you!\n";
        
        auto it = bookGenre[userGenre].rbegin();
        for(int i = 0; i < random && i < bookGenre[userGenre].size(); i++){
          it--;
        }
        cout << "Title: " << books[it->second].title << '\n';
        cout << "Genre: " << userGenre << '\n' << '\n';
      }
      break;
    }
    
    case 3:
    {
      // contains key func that looks for genre
      userGenre = "Romance";
      if(bookGenre.containsKey(userGenre))
      {
        cout << "We found a book for you!\n";
        
        auto it = bookGenre[userGenre].rbegin();
        for(int i = 0; i < random && i < bookGenre[userGenre].size(); i++){
          it--;
        }
        cout << "Title: " << books[it->second].title << '\n';
        cout << "Genre: " << userGenre << '\n' << '\n';
      }
      break;
    }
    
    case 4:
    {
      // contains key func that looks for genre
      userGenre = "Nonfiction";
      if(bookGenre.containsKey(userGenre))
      {
        cout << "We found a book for you!\n";
        auto it = bookGenre[userGenre].rbegin();
        for(int i = 0; i < random && i < bookGenre[userGenre].size(); i++){
          it--;
        }
        cout << "Title: " << books[it->second].title << '\n';
        cout << "Genre: " << userGenre << '\n' << '\n';
      }
      break;
    }
    default:
      cout << "Error please enter a number from 1 to 4\n";
      break;
  }

  cout << "Press [1] to Return to menu or enter [Q] or [q] to quit: ";
  cin >> buf;
  
  if(buf == "Q" || buf == "q")
  {
    return;
  }
  else if (buf == "1")
  {
    mainMenu(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
  }
}

/**
 * request a book
 *
 * @param books  the new book will be stored here
 * @param bookISBN  the new book's ISBN and index in books array will also be stored here
 * @param bookTitle  the new book's title and index in books array will also be stored here
 * @param bookSize the size of the book array.
 */
void requestBook(DynamicArray<book>& books, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, int& bookSize, string& userISBN)
{
  //fields
  book temp;

  //get info
  cin.ignore();
  cout << "Please enter the book Title: ";
  getline(cin, temp.title);
  cout << "Please enter the book Author: ";
  getline(cin, temp.author);
  cout << "Please enter the book ISBN Code: ";
  getline(cin, temp.ISBN);
  cout << "Please enter the book Genre: ";
  getline(cin, temp.genre);

  temp.numRating = 0;
  temp.totalRating = 0;
  temp.avaliable = 1;
    
  //store info
  books[bookSize] = temp;
  bookISBN[temp.ISBN] = bookSize;
  bookTitle[temp.title] = bookSize;
  bookSize++;
  
  cout << "Confirmed! The book has been added." << endl << endl;
  
  // cout << "Confirmed! You have requested " << books[bookISBN[userISBN]].title << endl;
}

/**
 * check out a book
 *
 * @param books  the avaliable of the book will be updated
 * @param users  the book and its date borrowed will be updated
 * @param currentUser  where the user is stored in the array
 */
void checkOut(DynamicArray<book>& books, DynamicArray<user>& users, int currentUser, int& bookIndex, HashTable<string, int, 75000>& bookISBN, string& userISBN)
{
  if(users[currentUser].bookBorrowed != "N/A")
  {
    cout << "Sorry, you have to return your borrowed book before borrowing a new book." << endl;
    return;
  }
  else if(!books[bookIndex].avaliable){
    cout << "Sorry, this book is unavailble at the moment." << endl;
    return;
  }
  else
  {
    cout << "Confirmed! You have checked out the book, you have 30 days to return it.\n";
  }

  //get time
  time_t t = time(0);
  struct tm * timeStruct = localtime(&t);
  int time = timeStruct->tm_yday + 356 * (timeStruct->tm_year);

  users[currentUser].borrow = time;
  users[currentUser].bookBorrowed = books[bookIndex].ISBN;
  
  books[bookIndex].avaliable = 0;
}

int hashCodeI(const unsigned int& key)
{
  return key % 10;
}

int hashCodeS(const string& key)
{
  int result = 0;
  for(int i = 0; i < key.length(); i++)
  {
    if(i < key.length() - 4)
    {
      result += key.at(i);
    }
    else
    {
      result += (int)(key.at(i) * pow(10, key.length() - i));
    }
  }
  return result;
}


void mainMenu(int currentUser, int bookIndex, DynamicArray<book>& books, DynamicArray<user>& users, HashTable<string, int, 75000>& bookISBN, HashTable<string, int, 75000>& bookTitle, bool& run, HashTable<string, map<double, int>, 75000>& bookGenre, int& bookSize, string& userISBN)
{
  string buf;
  int option;
  
  cout << endl << endl;
  cout << "**************************************************" << endl;
  cout << "* Main Menu:                                      *" << endl;
  cout << "*     [1] Search Books                            *" << endl;
  cout << "*     [2] Return Books                            *" << endl;
  cout << "*     [3] Get Recommendations                     *" << endl;
  cout << "*     [4] Log out                                 *" << endl;
  cout << "**************************************************" << endl << endl;

  cout << "Please enter your choice: ";
  cin >> buf;
  option = atoi(buf.c_str());
  cout << endl;

  // invalid input
  while(option != 1 && option != 2 && option != 3 && option != 4)
  {
    cout << "Invalid input, please enter again: ";
    cin >> buf;
    option = atoi(buf.c_str());
  }

  switch (option)
  {
    //search book
    case 1:
      searchBook(currentUser, bookIndex, books, users, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
      break;
      
    //return book
    case 2:
      returnBook(bookIndex, books, users, currentUser, bookISBN, bookTitle, run, bookGenre, bookSize, userISBN);
      break;
      
    //get recommendation
    case 3:
      getRecommendation(users, currentUser, books, bookGenre, bookIndex, bookISBN, bookTitle, run, bookSize, userISBN);
      break;
       
    //quit
    default:
      break;
   }
}
