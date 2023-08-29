/// hello.cpp
/// This file contains the source code for the example in Chapter 3
/// in charge of reading LLVM bitcode files and printing the name of
/// the functions defined in it.

#include "llvm/IR/LLVMContext.h"
#include "llvm/Bitcode/ReaderWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Support/system_error.h"
#include <iostream>
using namespace llvm;

static cl::opt<std::string>
FileName(cl::Positional, cl::desc("Bitcode file"),
         cl::Required);

int main(int argc, char** argv)
{
  cl::ParseCommandLineOptions(argc, argv, "LLVM hello world\n");
  LLVMContext context;
  
  OwningPtr<MemoryBuffer> mb;
  MemoryBuffer::getFile(FileName, mb);

  std::string error;
  Module* m = ParseBitcodeFile(mb.get(), context, &error);
  if (m == 0) {
    std::cerr << "Error reading bitcode: " << error << "\n";
    return -1;
  }

  raw_os_ostream O(std::cout);

  for (Module::const_iterator i = m->getFunctionList().begin(),
    e = m->getFunctionList().end(); i != e; ++i) 
    {
    if (!i->isDeclaration()) 
      {
        O << i->getName() << " has " << i->size() << " basic blocks.\n";
      }
  }
  return 0;
}
