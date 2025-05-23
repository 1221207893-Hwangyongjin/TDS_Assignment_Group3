# TDS_Assignment_Group3

# Swimming Equipment and Swimwear Management System
# Overview
This is a comprehensive management system for swimming equipment and swimwear products, developed by Team A and Team B. The system provides functionalities for product management, including adding, searching, sorting, and saving product information.

# Features
Team A Implementation (LinkedQueue + Hash Table)
1. Product Management

 - Add new products with auto-generated unique IDs

 - View all products in insertion order

 - Search products by ID (using hash table for O(1) average lookup)

 - Save products to file

2. Data Structures

 - Queue (FIFO) for maintaining insertion order

 - Separate Chaining Hash Table for efficient searching

 - Automatic collision handling

3. File Operations

 - Load products from "products.txt"

 - Save sorted products to "TeamA_sorted_information.txt"
 - 

# Team B Implementation (Stack + Sorting/Searching)
1. Product Management

Add/remove products from stack

Display all products

Sort products by various fields (ID, name, category, etc.)

Search products by ID using Meta Binary Search

2. Data Structures

Array-based stack implementation

Gnome Sort algorithm for sorting

Meta Binary Search for efficient searching

3. File Operations

Load products from "products.txt"

Save sorted products to "TeamB_sorted_information.txt"

## Data Structure Details ##
# Team A
1. Separate Chaining Hash Table

 - Size: 100 buckets

 - Hash function: Simple modulo operation

 - Collision resolution: Linked list chaining

2. Linked Queue

 - Maintains product insertion order

 - FIFO (First-In-First-Out) operations

# Team B
1. Array-based Stack

 - Fixed capacity of 100 products

 - LIFO (Last-In-First-Out) operations

2. Sorting Algorithm

 - Gnome Sort implementation

 - Supports sorting by multiple fields in ascending/descending order

3. Searching Algorithm

 - Meta Binary Search for efficient lookups

 - Requires sorted data by ID

# How to Use
1. Main Menu

 - Choose between Team A or Team B functionality

 - Exit the program

2. Team A Features

 - View all products

 - Add new products

 - Search by ID

 - Save to file

 - Display sorted data

 - View hash table contents

# Team B Features

 - Display unsorted products

 - Add/remove products

 - Sort by various fields

 - Search by ID

 - Save to file

# File Formats
Input File ("products.txt")
ID,Name,Category,Size,Color,Price
101,Swim Goggles,Accessories,Medium,Black,25.50
102,Swim Cap,Accessories,One Size,Blue,15.00
...
Output Files
TeamA_sorted_information.txt

TeamB_sorted_information.txt

# Technical Details
 - Language: C++

 - Data Structures: Hash Table, Queue, Stack

 - Algorithms: Gnome Sort, Meta Binary Search

 - Input/Output: File handling with ifstream/ofstream




# Notes
The system generates unique product IDs based on timestamp

Both teams maintain their own sorted output files

The hash table provides O(1) average case time complexity for searches

Meta Binary Search provides efficient O(log n) search time on sorted data

# Future Enhancements
Implement a unified interface for both teams

Add data validation for user inputs

Support for larger datasets

Additional search criteria (name, category, etc.)

Graphical user interface

