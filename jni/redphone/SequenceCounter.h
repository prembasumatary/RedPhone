#ifndef __SEQUENCE_COUNTER_H__
#define __SEQUENCE_COUNTER_H__

class SequenceCounter {

private:
  int64_t currentLongId;

public:
  SequenceCounter() : currentLongId(0) {}

  int64_t getNextLogicalSequence(int16_t nextShortId) {
    int16_t smallestDeltaToCongruentId = (int16_t)(nextShortId - currentLongId);
    currentLongId += smallestDeltaToCongruentId;

    return currentLongId;
  }
};


#endif