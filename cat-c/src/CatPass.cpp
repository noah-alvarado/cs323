#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

using namespace std;
using namespace llvm;

namespace {
  struct CAT : public FunctionPass {
    static char ID; 

    CAT() : FunctionPass(ID) {}

    // This function is invoked once at the initialization phase of the compiler
    // The LLVM IR of functions isn't ready at this point
    bool doInitialization (Module &M) override {
      return false;
    }

    // This function is invoked once per function compiled
    // The LLVM IR of the input functions is ready and it can be analyzed and/or transformed
    bool runOnFunction (Function &F) override {
      DominatorTree &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
      std::map<std::string, int> counters;

      for (auto& B : F) {
        if (DT.dominates(&*B.begin(), &B)) continue;

        for (auto& I : B) {
          if (isa<CallInst>(&I)) {
            CallInst *callInst = cast<CallInst>(&I);
            Function *callee = callInst->getCalledFunction();

            std::string funcName = callee->getName();
            if (funcName.substr(0, 3) == "CAT") {
              if (counters.count(funcName) == 0) {
                counters[funcName] = 0;
              }
              counters[funcName] = counters[funcName] + 1;
            }
          }
        }
      }

      if (counters.count("CAT_add") > 0) errs() << "H1: \"" << F.getName() << "\": CAT_add: "  << counters["CAT_add"] << "\n";
      if (counters.count("CAT_sub") > 0) errs() << "H1: \"" << F.getName() << "\": CAT_sub: "  << counters["CAT_sub"] << "\n";
      if (counters.count("CAT_new") > 0) errs() << "H1: \"" << F.getName() << "\": CAT_new: "  << counters["CAT_new"] << "\n";
      if (counters.count("CAT_get") > 0) errs() << "H1: \"" << F.getName() << "\": CAT_get: "  << counters["CAT_get"] << "\n";
      if (counters.count("CAT_set") > 0) errs() << "H1: \"" << F.getName() << "\": CAT_set: "  << counters["CAT_set"] << "\n";

      return false;
    }

    // We don't modify the program, so we preserve all analyses.
    // The LLVM IR of functions isn't ready at this point
    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.setPreservesAll();
    }
  };
}

// Next there is code to register your pass to "opt"
char CAT::ID = 0;
static RegisterPass<CAT> X("CAT", "Homework for the CAT class");

// Next there is code to register your pass to "clang"
static CAT * _PassMaker = NULL;
static RegisterStandardPasses _RegPass1(PassManagerBuilder::EP_OptimizerLast,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CAT());}}); // ** for -Ox
static RegisterStandardPasses _RegPass2(PassManagerBuilder::EP_EnabledOnOptLevel0,
    [](const PassManagerBuilder&, legacy::PassManagerBase& PM) {
        if(!_PassMaker){ PM.add(_PassMaker = new CAT()); }}); // ** for -O0
