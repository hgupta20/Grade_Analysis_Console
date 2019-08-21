/*main.cpp*/

//
// Harsh Devprakash Gupta
// U. of Illinois, Chicago
// CS 341, Spring 2019
// Project #03: GradeUtil Console

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <algorithm>
#include <functional>

#include "gradeutil.h"
using namespace std;
College sortDepartment(College A)
{
    sort(A.Depts.begin(), A.Depts.end(),
         [](Dept a, Dept b) {
             if (a.Name > b.Name)
                 return false;
             else
                 return true;
         });
    return A;
}
void summary(College collegeName)
{
    // print the dept name or all
    cout << "dept name, or all? ";
    string input;
    cin >> input;
    // use the Input and compare if it is all
    if (input == "all")
    {
        College sortedCollege = sortDepartment(collegeName);
        for (Dept i : sortedCollege.Depts)
        {
            int DFW, N;
            i.courseSummary();
            GradeStats grades = GetGradeDistribution(i);
            cout << " grade distribution (A-F): " << grades.PercentA << "%, " << grades.PercentB << "%, " << grades.PercentC << "%, " << grades.PercentD << "%, " << grades.PercentF << "%" << endl;
            cout << " DFW rate: " << GetDFWRate(i, DFW, N) << "%" << endl;
        }
    }
    else
    {
        auto m = find_if(collegeName.Depts.begin(), collegeName.Depts.end(),
                            [=](Dept D) {
                                if (D.Name == input)
                                    return true;
                                    
                                else
                                    return false;
                            });
        // compare if the vector ended
        if (m == collegeName.Depts.end())
        {
            cout << "**dept not found" << endl;
        }
        else
        {
            int DFW, N;
            m->courseSummary();
            GradeStats grades = GetGradeDistribution(*m);
            cout << " grade distribution (A-F): " << grades.PercentA << "%, " << grades.PercentB << "%, " << grades.PercentC << "%, " << grades.PercentD << "%, " << grades.PercentF << "%" << endl;
            cout << " DFW rate: " << GetDFWRate(*m, DFW, N) << "%" << endl;
        }
    }
}
void summary(Course C)
{
    // Print all the required data according to the handout
    int DFW, N;
    cout << C.Dept << " " << C.Number << " (section " << C.Section << ")";
    cout << ": " << C.Instructor << endl;
    cout << " # students: " << C.getNumStudents() << endl;
    cout << " course type: ";
    if (C.getGradingType() == Course::Letter)
    {
        cout << "letter";
    }
    else if (C.getGradingType() == Course::Unknown)
    {
        cout << "unknown";
    }
    else
    {
        cout << "satisfactory";
    }
    cout << endl;
    GradeStats grades = GetGradeDistribution(C);
    cout << " grade distribution (A-F): " << grades.PercentA << "%, " << grades.PercentB << "%, " << grades.PercentC << "%, " << grades.PercentD << "%, " << grades.PercentF << "%" << endl;
    cout << " DFW rate: " << GetDFWRate(C, DFW, N) << "%" << endl;
}

vector<Course> sortDFW(vector<Course> dfwVec)
{
    sort(dfwVec.begin(), dfwVec.end(),
         [](Course s1, Course s2) {
             int DFW, N;
             if (GetDFWRate(s1, DFW, N) < GetDFWRate(s2, DFW, N))
                 return false;
             else if (GetDFWRate(s2, DFW, N) < GetDFWRate(s1, DFW, N))
                 return true;

             else
             {
                 if (s1.Dept < s2.Dept)
                     return true;
                 else if (s1.Dept > s2.Dept)
                     return false;
                 else
                     if (s1.Number < s2.Number)
                         return true;
                     else if (s1.Number > s2.Number)
                         return false;
                     else
                         if (s1.Section < s2.Section)
                             return true;
                          else
                             return false;


             }
         });
    return dfwVec;
}

vector<Course> sortPercentVector(vector<Course> percentVec)
{
    // use sort to sort the vector
    sort(percentVec.begin(), percentVec.end(),
         [=](Course s1, Course s2) {
             GradeStats A;
             GradeStats B;
             A = GetGradeDistribution(s1);
             B =GetGradeDistribution(s2);

             if (A.PercentA >B.PercentA)
                 return true;
             else if (A.PercentA <B.PercentA)
                 return false;
            else
            {
                 if (s1.Dept < s2.Dept)
                     return true;
                 else if (s1.Dept >s2.Dept)
                     return false;
                 else
                 {
                     if (s1.Number < s2.Number)
                         return true;
                     else if (s1.Number > s2.Number)
                         return false;
                     else
                     {
                         if (s1.Section < s2.Section)
                             return true;
                          else
                             return false;
                     }
                 }
           }
         });
    return percentVec;
}


vector<Course> findDFW(Dept A, double DFW)
{
    // function to find DFW
    vector<Course> dfwVec;
    int DFW1, N;
    for (Course B : A.Courses)
        {            
            double tempDFW = GetDFWRate(B, DFW1, N);
            if (tempDFW > DFW)
            {
                dfwVec.push_back(B);
            }
        }
    return sortDFW(dfwVec);
}

vector<Course> findDFW(College A, double DFW)
{
    vector<Course> dfwVec;
    int DFW1, N;
    for (Dept C : A.Depts)
    {
        for (Course B : C.Courses)
        {
            double tempDFW = GetDFWRate(B, DFW1, N);
            if (tempDFW > DFW)
            {
                dfwVec.push_back(B);
            }
        }
    }
    return sortDFW(dfwVec);
}
vector<Course> findLetter(College collegeName, double threshold)
{
        // function for find letter using collegeName
        GradeStats G;
        vector<Course> letterVec;
        for(Dept i : collegeName.Depts)
            {   
                for(Course j : i.Courses)
                    {
                        G = GetGradeDistribution(j);
                        if(G.PercentA > threshold)
                            {
                                letterVec.push_back(j);
                            }
                    }
            }
            return sortPercentVector(letterVec);  

}
vector<Course> findLetter(Dept A, double threshold)
{
        // function for find letter using Dept
        GradeStats G;
        vector<Course> letterVec;
        // iterate through the vectors
        for(Course j : A.Courses)
                    {
                        G = GetGradeDistribution(j);
                        if(G.PercentA > threshold)
                            {
                                letterVec.push_back(j);
                            }
                    }
            return sortPercentVector(letterVec);  
}
void letterA(College collegeName)
{
        // print according to the handout and input the file
        cout << "dept name, or all? ";
        string input;
        cin >> input;
        // vector to store threshold
        vector<Course> vec;
        double threshold;
        if(input == "all")
            {
                cout << "letter A threshold? ";
                cin >> threshold;
                vec = findLetter(collegeName, threshold);

            }
        else
        {
            // use find_if to find the Dept
            auto m = find_if(collegeName.Depts.begin(), collegeName.Depts.end(),
                            [=](Dept A) {
                                if (A.Name == input)
                                    return true;
                                else
                                    return false;
                            }

        );
         cout << "letter A threshold? ";
                cin >> threshold;
        if (m == collegeName.Depts.end())
          {
            cout << "**dept not found" << endl;
            return;
          }
        else
          {
                vec = findLetter(*m, threshold);
          }
        }
        // if the vector is empty then return **none found
        if(vec.empty())
            {
                cout << "**none found" << endl;
                return;
            }
        // print the summary using the summary function
        for(Course i : vec)
        {
            summary(i);
        }


    }
void unknown(College collegeName)
{
    cout << "dept name, or all? ";
    string input;
    cin >> input;

    if (input == "all")
    {
        vector<Course> vec = collegeName.findUnknown();
        for (Course i : vec)
        {
            summary(i);
        }
    }
    else
    {
        auto m = find_if(collegeName.Depts.begin(), collegeName.Depts.end(),
                            [=](Dept A) {
                                if (A.Name == input)
                                    return true;
                                else
                                    return false;
                            }

        );
        if (m == collegeName.Depts.end())
        {
               cout  << "**none found" << endl;
               return;
        }
        else
        {
            /*auto m = find_if(collegeName.Depts.begin(), collegeName.Depts.end(),
                            [=](Dept A) {
                                if (A.Name == input)
                                    return true;
                                else
                                    return false;
                            }

        );*/
            vector<Course> vec = m->findUnknown();
            if(vec.size() == 0)
                {
                   cout  << "**none found" << endl;
                    return; 
                }
            for (Course i : vec)
            {
                summary(i);
            }
        }
    }
    return;
}
void dfwCommand(College collegeName)
{
    cout << "dept name, or all? ";
    string input;
    cin >> input;

    double DFW;
    vector<Course> vec;
    if (input == "all")
    {
        cout << "dfw threshold? ";
        cin >> DFW;
        vec = findDFW(collegeName, DFW);
       
    }
    else
    {
        cout << "dfw threshold? ";
        cin >> DFW;
     auto m = find_if(collegeName.Depts.begin(), collegeName.Depts.end(),
                            [=](Dept A) {
                                if (A.Name == input)
                                    return true;
                                else
                                    return false;
                            }

        );
        if (m == collegeName.Depts.end())
        {
              cout << "**dept not found" << endl;
              return;
        }
        else
        {
            vec = findDFW(*m,DFW);   
        }
            /*if (vec.empty())
        {
            cout <<"**none found" << endl;
            return;
        }*/
    }
    if (vec.empty())
        {
            cout <<"**none found" << endl;
            return;
        }
    else{
     for (Course i : vec)
        {
            summary(i);
        }
    }
}
void searchCommand(College collegeName)
{
    // print the dept name or all
    cout << "dept name, or all? ";
    string instructorPrefix; // store instructorPrefix
    int courseNum;
    string input;
    cin >> input;

    if (input == "all") // compare if all
    {
        cout << "course # or instructor prefix? ";
        cin >> instructorPrefix;
        stringstream ss(instructorPrefix);
        ss >> courseNum;
            
        if ((ss.fail())) // if it fails according to the handout we show none found
        {
            vector<Course> vec = FindCourses(collegeName, instructorPrefix);
            if(vec.size()==0)
                {
                    cout << "**none found" << endl;
                    return;
                }
            // print the summary 
            for (Course i : vec)
            {
                summary(i);
            }
        }
        else
        {
            vector<Course> vec = FindCourses(collegeName, courseNum);
            // check if it is found
            if(vec.size()==0)
                {
                    cout << "**none found" << endl;
                    return;
                }
            // print the summary 
            for (Course i : vec)
            {
                summary(i);
            }
        }
    }
    else
    {
        cout << "course # or instructor prefix? ";
        cin >> instructorPrefix;
        // use find if to find the instructor
        auto m = find_if(collegeName.Depts.begin(), collegeName.Depts.end(),
                            [=](Dept A) {
                                if (A.Name == input)
                                    return true;
                                 else
                                    return false;
                            }

        );
        if (m == collegeName.Depts.end())
        {
            cout << "**dept not found" << endl;
        }
            /*stringstream ss(instructorPrefix);
            ss >> courseNum;
            if ((ss.fail()))
            {
                vector<Course> vec = FindCourses(*m, instructorPrefix);
                if(vec.size()==0)
                {
                    cout << "**none found" << endl;
                    return;
                }
                for (Course i : vec)
                {
                    summary(i);
                }
            }*/
        else
        {
            stringstream ss(instructorPrefix);
            ss >> courseNum;
            if ((ss.fail()))
            {
                vector<Course> vec = FindCourses(*m, instructorPrefix);
                if(vec.size()==0)
                {
                    cout << "**none found" << endl;
                    return;
                }
                for (Course i : vec)
                {
                    summary(i);
                }
            }
            else
            {
                vector<Course> vec = FindCourses(*m, courseNum);
                if(vec.size()==0)
                {
                    cout << "**none found" << endl;
                    return;
                }
                for (Course i : vec)
                {
                    summary(i);
                }
            }
        }
        return;
    }
}





// MAIN Starts HERE!!!!!
int main(int argc, char *argv[])
{
    // All real numbers are output with 2 digits to the right of the decimal point. 
    cout << std::fixed;
    cout << std::setprecision(2);
        
    // String to store the filename
    string filename;
    cin >> filename;
        
    ifstream infile(filename);
    string line;
    if (!infile.good()) // check if the file is good
    {
        cout << "**Error: cannot open input file!" << endl;
        return 0;
    }
    string name;
    getline(infile, line);
    // print the data on the handout
    cout << "**  College of ";
    stringstream ss(line);
    getline(ss, name, ',');
    cout << name;
    
    
    getline(ss, name, ',');
    cout << ", " << name;
    string year;
    getline(ss, year);
    cout << " " << year << " **";

    cout << endl;

    getline(infile, line);
    getline(infile, line);
    College A;

    while (!infile.eof())// till end of file
    {
        Course C = ParseCourse(line); // use the gradeutil function ParseCourse
        // use find if to find the Dept
        auto m = find_if(A.Depts.begin(), A.Depts.end(),
                            [=](Dept D) {
                                if (D.Name == C.Dept)
                                    return true;
                                else
                                    return false;
                            });
        if (m == A.Depts.end())
        {
            Dept L;
            L.Name = C.Dept;
            L.Courses.push_back(C);// add the courses to the Dept
            A.Depts.push_back(L);
        }
        else
        {
            m->Courses.push_back(C); 
        }
        getline(infile, line);
    }
    infile.close(); // close the file
    int numofClasses = 0;
    int numofStudents = 0;
    // to calculate the number of students, got to the departments and calculate numberof students
    for (Dept i : A.Depts)
    {
        numofClasses = numofClasses + i.Courses.size();
        for (Course j : i.Courses)
        {
            numofStudents = numofStudents + j.getNumStudents();
        }
    }
    // print the data according to the handout
    cout << "# of courses taught: " << numofClasses << endl;
    cout << "# of students taught: " << numofStudents << endl;
    // calculate the gradeDistribution
    GradeStats grades = GetGradeDistribution(A);
    // print the grade distribution
    cout << "grade distribution (A-F): " << grades.PercentA << "%, " << grades.PercentB << "%, " << grades.PercentC << "%, " << grades.PercentD << "%, " << grades.PercentF << "%" << endl;
    // initialize DFW and N
    int DFW, N;
    cout << "DFW rate: " << GetDFWRate(A, DFW, N) << "%" << endl << endl;
    cout << "Enter a command> ";
    // to store a command define a string
    string command;
    cin >> command;
    // while loop to keep it infinite
    while (command != "#")
    {
        
        // for summary command
        if (command == "summary")
        {
            summary(A);
        }
        // for search command
        else if (command == "search")
        {
            searchCommand(A);
        }
        // for unknown command
        else if (command == "unknown")
        {
            unknown(A);
        }
        // for dfw command
        else if (command == "dfw")
        {
            dfwCommand(A);
        }
        // for unknown command
        else if(command == "letterA")
        {
                letterA(A);
        }
        // for not found command command
        else{
                cout << "**unknown command" << endl;
        }
        // Enter a command - 
        cout <<"Enter a command> ";

        cin >> command;
    }

    return 0;
}