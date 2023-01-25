#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include <windows.h>

using namespace std;

// Funktion um das Auswahlmenü anzuzeigen.
string print_menu(){
   cout << "VOKABELTRAINER | By: Coding_Trashpanda\n\n";
   cout << "Bitte treffe eine Auswahl: \n\n";
   cout << "(1) Neue Vokabel hinzufügen\n";
   cout << "(2) Vokabeln üben\n";
   cout << "(3) Vokabeltest starten\n";
   cout << "(4) Multiple Choice Test starten\n";
   cout << "(5) Vokabeltrainer beenden.\n";
   string selection;
   cin >> selection;
   return selection;
}

// Funktion um die Text Dokumente auszulesen
vector<string> loadTextFile(string textFile){
   string str;
   ifstream in(textFile);
   vector<string> newVector;
   // Read the next line from File untill it reaches the end
   while (getline(in, str)) 
   {
      // Line contains string of length > 0 then save it in vector
      if(str.size() > 0)
         newVector.push_back(str);
   }
   return newVector;
}

// Speichert deutsche und spanische Vokabeln.
void save_vector_to_text_file(string fileName, vector<string> myVector){
      ofstream output_file(fileName);
      ostream_iterator<string> output_iterator(output_file, "\n");
      copy(myVector.begin(), myVector.end(), output_iterator);
}

// Funktion um die Statistik zu speichern.
void save_statistics(int correct_answers, int total_guestions, int score){
   time_t t = time(0);  
   struct tm * now = localtime( & t );

   ofstream output_file("statistics.txt", ios::app);                       

   output_file << (now->tm_year + 1900) << '-'
               << setfill('0') << setw(2) << (now->tm_mon + 1) << '-'
               << setfill('0') << setw(2) << now->tm_mday << " "
               << setfill('0') << setw(2) << now->tm_hour << ":"
               << setfill('0') << setw(2) << now->tm_min << " "
               << "\n" << "Richtige Antworten: " << correct_answers << endl;
   output_file << "Antworten insgesamt: " << total_guestions << endl;
   output_file << "Score: " << score << endl;
}

// Funktion um Vokabeln hinzuzufügen.
void add_vocab(vector<string> &voc_german, vector<string> &voc_spanish){
   string german_word, spanish_word;
   cout << "Bitte gebe die deutsche Vokabel ein: \n";
   cin >> german_word;
   cin.ignore();                                            // Sorgt dafür, dass die Eingabe vom spanischen Wort nicht übersprungen wird.
   cout << "Bitte gebe die spanische Übersetzung ein: \n";
   getline(cin, spanish_word);

   if(german_word != "" && spanish_word != ""){
      voc_german.push_back(german_word);
      voc_spanish.push_back(spanish_word);
      cout << "Die Vokabel wurde erfolgreich hinzugefügt! \n";
   } else {
      cout << "Es wurden nicht alle Felder ausgefüllt. Bitte versuche es erneut.\n";
   }
}

// Funktion zum Lernen der Vokabeln.
void ask_word(vector<string> voc_german, vector<string> voc_spanish){
   vector<int> asked;

   while (asked.size() < voc_german.size()) {
      int r;
      do {
         r = rand() % voc_german.size();
      } while(find(asked.begin(), asked.end(), r) != asked.end());
   
   
   asked.push_back(r);
   string sel_elem = voc_german[r];
   string sel_translation = voc_spanish[r];
   cout << "Bitte übersetze folgendes Wort: " + sel_elem + ": ";
   string userInput;
   cin >> userInput;
   if(userInput == sel_translation) 
   {
      cout << "Das ist richtig!\n";
   } else 
   {
      cout << "Das ist leider falsch!\n";
   }

   if (asked.size() == voc_german.size()) {
      cout << "Super! Alle Vokabeln wurden abgefragt, die Übung ist nun beendet. \n";
      break;
   }
   }

   // Sorgt dafür, dass "Viel Spaß beim lernen" nur ein einziges Mal ausgegeben wird.
   static bool firstRun = true;
   if (firstRun)
   {
      cout << "Viel Spaß beim lernen!\n\n";
      firstRun = false;
   }
}

// Funktion für einen Vokabeltest inkl. Ausgabe der Note.
int check_vocab(vector<string> voc_german, vector<string> voc_spanish){
   int correct = 0;
   int incorrect = 0;

   // Zeigt an wieviele Antworten richtig oder falsch sind.
   for(int i = 0; i < voc_german.size(); i++){
      cout << "Wie lautet die Übersetzung von " << voc_german[i] << "?";
      string translation;
      cin >> translation;
      if(translation == voc_spanish[i]){
         correct++;
      } else {
         incorrect++;
      }
   }

   // Zeigt die Note an, gemessen anhand der richtigen und falschen Antworten.
   float score = (float) correct / (float) voc_german.size() * 100;
   int grade = 0;
   if (score >= 90) {
      grade = 1;
   } else if (score >= 80) {
      grade = 2;
   } else if (score >= 70) {
      grade = 3;
   } else if (score >= 60) {
      grade = 4;
   } else if (score >= 50) {
      grade = 5;
   } else {
      grade = 6;
   } 

   cout << "Richtig: " << correct << ", Falsch: " << incorrect << endl;
   cout << "Note: " << grade << endl;

   return grade;
}

//Funktion für den Multiple Choice Test
void multi_choice_test(vector<string> voc_german, vector<string> voc_spanish) {
   int correct_answers = 0;
   int total_questions = 0;
   int score = 0;
   for (int i = 0; i < voc_german.size(); i++) {
      total_questions++;
      cout << "Bitte wähle die richtige Übersetzung für folgendes Wort: " << voc_german[i] << endl;

      // Create a vector of possible answers
      vector<string> possible_answers;
      possible_answers.push_back(voc_spanish[i]);
      for (int j = 0; j < 4; j++) {
         int random_index = rand() % voc_spanish.size();
         if (voc_spanish[random_index] != voc_spanish[i]) {
            possible_answers.push_back(voc_spanish[random_index]);
         }
      }
      random_shuffle(possible_answers.begin(), possible_answers.end());

      // Print out the possible answers
      for (int j = 0; j < possible_answers.size(); j++) {
         cout << j+1 << ": " << possible_answers[j] << endl;
      }

      // Get the user's choice
      int user_choice;
      cin >> user_choice;
      if (possible_answers[user_choice - 1] == voc_spanish[i]) {
         cout << "Das ist richtig!\n";
         correct_answers++;
         score++;
      } else {
         cout << "Das ist leider falsch. Die richtige Übersetzung ist: " << voc_spanish[i] << endl;
      }
   }
   
   cout << "Du hast " << correct_answers << " von " << total_questions << " Fragen richtig beantwortet.\n";
   cout << "Dein Score ist " << score << " von " << total_questions << ".\n";
   save_statistics(correct_answers, total_questions, score);
}

int main() {
   vector<string> voc_german = loadTextFile("voc_german.txt");
   vector<string> voc_spanish = loadTextFile("voc_spanish.txt");

   while (true) {
   string selection = print_menu();

   if(selection == "1"){
      add_vocab(voc_german, voc_spanish);
      save_vector_to_text_file("./voc_german.txt", voc_german);
      save_vector_to_text_file("./voc_spanish.txt", voc_spanish);
   } else if (selection == "2") {
      int i = 0;
      cout << "Viel Spaß beim lernen!\n\n";
      while (i < 10) {
         ask_word(voc_german, voc_spanish);
         i++;
      }
   } else if ( selection == "3") {
      check_vocab(voc_german, voc_spanish);
   } else if ( selection == "4") {
      multi_choice_test(voc_german, voc_spanish);
   } else if ( selection == "5") {
      break;
   } else {
      cout << "Ungültige Eingabe, bitte treffe eine gültige Auswahl.\n";
   }
}
   return 0;
}
