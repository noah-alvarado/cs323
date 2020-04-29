#include "llvm/Pass.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <set>
#include <map>

using namespace std;
using namespace llvm;

namespace {
  struct CAT : public FunctionPass {
    static char ID;

    enum cat_code {
      cat_add,
      cat_sub,
      cat_new,
      cat_get,
      cat_set,
      none
    };

    cat_code funcToCatCode (std::string str) {
      if (str == "CAT_add") return cat_add;
      if (str == "CAT_sub") return cat_sub;
      if (str == "CAT_new") return cat_new;
      if (str == "CAT_get") return cat_get;
      if (str == "CAT_set") return cat_set;
      return none;
    };

    void printH2 (Function &F, std::map<Instruction *, std::set<Instruction *>> &gen_sets, std::map<Instruction *, std::set<Instruction *>> &kill_sets) {
      errs() << "Function \"";
      errs().write_escaped(F.getName()) << "\"\n";

      for (auto& B : F) {
        for (auto& I : B) {
          if (gen_sets.find(&I) == gen_sets.end()) continue;

          errs() << "INSTRUCTION:\t";
          I.print(errs());
          errs() << "\n";

          errs() << "***************** GEN" << "\n";
          errs() << "{" << "\n";
          // TODO: print stuff in GEN
          errs() << "}" << "\n";

          errs() << "**************************************" << "\n";

          errs() << "***************** KILL" << "\n";
          errs() << "{" << "\n";
          // TODO: print stuff in KILL
          errs() << "}" << "\n";

          errs() << "**************************************" << "\n\n\n\n";
        }
      }
    }

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

      std::map<Instruction *, std::set<Instruction *>> gen_sets;
      std::map<Instruction *, std::set<Instruction *>> kill_sets;

      for (auto& B : F) {
        if (DT.dominates(&*B.begin(), &B)) continue;

        for (auto& I : B) {
          // Initialize gen and kill sets
          gen_sets[&I].clear();
          kill_sets[&I].clear();

          if (isa<CallInst>(&I)) {
            CallInst *callInst = cast<CallInst>(&I);
            Function *callee = callInst->getCalledFunction();

            std::string funcName = callee->getName();
            switch (funcToCatCode(funcName)) {
              case cat_add: {
                break;
              }
              case cat_sub: {
                break;
              }
              case cat_new: {
                gen_sets[&I].insert(&I);
                break;
              }
              case cat_get: {
                break;
              }
              case cat_set: {
                break;
              }
            }
          }
        }
      }

      printH2(F, gen_sets, kill_sets);

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
