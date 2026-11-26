#ifndef BATCH_H
#define BATCH_H

#include "inventory.h"

void batchMenu(Inventory *inv);
void addBatch(Inventory *inv);
void viewBatches(Inventory *inv);
void viewBatchesByProduct(Inventory *inv);
void updateBatch(Inventory *inv);
void deleteBatch(Inventory *inv);

#endif