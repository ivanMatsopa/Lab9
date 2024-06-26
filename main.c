#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
  int	id;
  char name;
  int	order; 
  struct RecordType* next;
};

// Fill out this structure
struct HashType
{
  struct RecordType **records;
};

// Compute the hash function
int hash(int x)
{
   return x % 10;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
  FILE* inFile = fopen(inputFileName, "r");
  int dataSz = 0;
  int i, n;
  char c;
  struct RecordType *pRecord;
  *ppData = NULL;

  if (inFile)
  {
    fscanf(inFile, "%d\n", &dataSz);
    *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
    // Implement parse data block
    if (*ppData == NULL)
    {
      printf("Cannot allocate memory\n");
      exit(-1);
    }
    for (i = 0; i < dataSz; ++i)
    {
      pRecord = *ppData + i;
      fscanf(inFile, "%d ", &n);
      pRecord->id = n;
      fscanf(inFile, "%c ", &c);
      pRecord->name = c;
      fscanf(inFile, "%d ", &n);
      pRecord->order = n;
    }

    fclose(inFile);
  }

  return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
  int i;
  printf("\nRecords:\n");
  for (i = 0; i < dataSz; ++i)
  {
    printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
  }
  printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
  int i;

  for (i=0;i<hashSz;++i)
  {
    // if index is occupied with any records, print all
    if (pHashArray->records[i] != NULL)
    {
        printf("Index %d -> ", i);
        struct RecordType *record = pHashArray->records[i];
        while (record != NULL)
        {
            printf("%d %c %d", record->id, record->name, record->order);
            if (record->next != NULL)
                printf(" -> ");
            record = record->next;
          }
          printf("\n");
    }  
  }
}

int main(void)
{
  struct RecordType *pRecords;
  int recordSz = 0;

  recordSz = parseData("input.txt", &pRecords);
  printRecords(pRecords, recordSz);
  // Your hash implementation
  int hashSize = 10; 
  struct HashType hashTable;
  hashTable.records = (struct RecordType **)malloc(sizeof(struct RecordType *) * hashSize);
  for (int i = 0; i < hashSize; i++)
  {
      hashTable.records[i] = NULL; // Initializing all the pointers to NULL
  }

  // Storing records in the hash table
  for (int i = 0; i < recordSz; i++)
  {
      int hashedIndex = hash(pRecords[i].id); // Hash ID records
      // Add the record to the linked list at the hashed index
      pRecords[i].next = hashTable.records[hashedIndex];
      hashTable.records[hashedIndex] = &pRecords[i];
  }

  // Display records in the hash table
  displayRecordsInHash(&hashTable, hashSize);

  // Free memory
  for (int i = 0; i < recordSz; i++)
  {
      free(&pRecords[i]);
  }
  free(pRecords);
  free(hashTable.records);

  return 0;
}
