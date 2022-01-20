# Disease Monitoring Application

This application accepts, processes, records and answers questions about virus cases. Specifically, it is implemented a set of structures (hash
tables, linked lists, binary trees) that allow the entry and queries in a large volume of records of ```patientRecord``` type. Although the data will come from files, eventually all
records will be stored in main memory only.

## Application Interface

The application will be called ```diseaseMonitor``` and will be used as follows: 

```./diseaseMonitor -p patientRecordsFile –h1 diseaseHashtableNumOfEntries –h2 countryHashtableNumOfEntries –b bucketSize```

where: 

- The ```diseaseHashtableNumOfEntries``` parameter is the number of entries in a hashtable that the application will hold to detect patient information.
- The ```countryHashtableNumOfEntries``` parameter is the number of entries in a hashtable that the application will hold to detect positive cases per country. 
- The ```bucketSize``` parameter is the number of bytes that defines the size of each bucket of the hash tables.
- The ```PatientRecordsFile``` (or some other filename) is a file that contains a number of patient records for processing. Each line of this file describes a case of a
the patient's name, in which country, the date of admission to the hospital and the date he was discharged. For example if the contents of the file are:


  ```889 Mary Smith COVID-2019 China 25-1-2019 27-1-2019```

  ```776 Larry Jones SARS-1 Italy 10-02-2003 –```

  ```125 Jon Dupont H1N1 USA 12-02-2016 15-02-2016```

means that we have three records describing three cases in three different countries
(China, Italy, USA). In the second registration, there is no discharge date (the patient remains at
hospital). Specifically, a record is an ASCII line of text that consists of the following data:

1. ```recordID```: a string (it can have only one digit) that in a unique way
determines each such record.
2. ```patientFirstName```: a string consisting of letters without spaces.
3. ```patientLastName```: a string consisting of letters without spaces.
4. ```diseaseID```: a string consisting of letters, numbers, and possibly one hyphen “-” but without spaces.
5. ```country```: a string consisting of letters without spaces.
6. ```entryDate```: date of admission to the hospital. It must have the form DD-MM-YYYY where DD expresses the day, MM the month, and YYYY the year that 
the patient admitted to the hospital.
7. ```exitDate```: date the patient was discharged from the hospital. It must have the form DD-MM-YYYY where DD expresses the day, MM the month, and YYYY the year that the patient discharged from the hospital or hyphen (-) which means that the patient has not yet been discharged.

## Application Functionality

In the beginning, the application opens the ```patientRecordFile```, reads the lines one by one, initializes and stores in memory the data structures that will be used when executing the queries. Also, it checks the validity of the data in the file. For example, it can handle errors such as two records in the ```patientRecordFile``` with the same ```recordID```. Moreover, if it finds a record where the ```entryDate``` is later than ```exitDate```, then the application will display a message that the record is rejected. In particular, the applications handles the below errors:

When the application finishes processing the ```patientRecordsFile``` file, it will wait user input from the keyboard. The user will be able to give the following commands (arguments in [] are optional):

- ```/globalDiseaseStats [date1 date2]```:

    The application will print for each virus, the number of cases recorded in the system. If
  ```date1``` ```date2``` are provided, then the application will print for each virus the number of cases they have been
  recorded in the system within the time period ```[date1 ... date2]```.
  If there is a definition for ```[date1]```, then there should be a definition for ```[date2]``` as well, otherwise 
  an error message is printed to the user.

- ```/diseaseFrequency virusName [country] date1 date2```:

  If 






