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

- 
-
- 
