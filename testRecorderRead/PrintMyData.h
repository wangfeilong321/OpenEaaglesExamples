
#ifndef __PrintMyData_H__
#define __PrintMyData_H__

#include "openeaagles/recorder/PrintHandler.h"

//------------------------------------------------------------------------------
// Class: PrintMyData
// Description: Print my extended recorder data
//
// Factory name: PrintMyData
//------------------------------------------------------------------------------
class PrintMyData : public oe::recorder::PrintHandler
{
   DECLARE_SUBCLASS(PrintMyData, oe::recorder::PrintHandler)

public:
   PrintMyData();

protected:
   virtual void processRecordImp(const oe::recorder::DataRecordHandle* const handle) override;

private:
   void initData();
};

#endif
