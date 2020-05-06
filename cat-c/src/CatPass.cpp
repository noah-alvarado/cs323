#include "llvm/Pass.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

#include <algorithm>
#include <map>
#include <set>

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

    void printH3 (Function &F, std::map<Instruction *, std::set<Instruction *>> &in_sets, std::map<Instruction *, std::set<Instruction *>> &out_sets) {
      errs() << "Function \"";
      errs().write_escaped(F.getName()) << "\" \n";

      for (auto& B : F) {
        for (auto& I : B) {
          if (in_sets.find(&I) == in_sets.end()) continue;

          errs() << "INSTRUCTION: ";
          I.print(errs());
          errs() << "\n";

          errs() << "***************** IN" << "\n";
          errs() << "{" << "\n";
          for (auto i : in_sets[&I]) {
            errs() << " ";
            i->print(errs());
            errs() << "\n";
          }
          errs() << "}" << "\n";

          errs() << "**************************************" << "\n";

          errs() << "***************** OUT" << "\n";
          errs() << "{" << "\n";
          for (auto i : out_sets[&I]) {
            errs() << " ";
            i->print(errs());
            errs() << "\n";
          }
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

      std::map<Instruction *, std::set<Instruction *>> in_sets;
      std::map<Instruction *, std::set<Instruction *>> out_sets;

      // Build gen and kill sets
      for (auto& B : F) {
        if (DT.dominates(&*B.begin(), &B)) continue;

        // build gen and kill sets
        // initialize in and out sets
        for (auto& I : B) {
          // Initialize gen and kill sets
          gen_sets[&I].clear();
          kill_sets[&I].clear();

          // Initialize in and out sets
          in_sets[&I].clear();
          out_sets[&I].clear();

          if (isa<CallInst>(&I)) {
            CallInst *callInst = cast<CallInst>(&I);
            Function *callee = callInst->getCalledFunction();

            std::string funcName = callee->getName();
            switch (funcToCatCode(funcName)) {
              case cat_add: {
                gen_sets[&I].insert(&I);

                Value* result = callInst->getArgOperand(0);
                if (isa<CallInst>(result)) {
                  kill_sets[&I].insert(cast<CallInst>(result));
                  kill_sets[cast<CallInst>(result)].insert(&I);
                }

                for (auto user : result->users()) {
                  if (isa<CallInst>(user)) {
                    CallInst *ci = cast<CallInst>(user);
                    if (result == ci->getArgOperand(0)) {
                      switch (funcToCatCode(ci->getCalledFunction()->getName())) {
                        case cat_add:
                          kill_sets[&I].insert(ci);
                          break;
                        case cat_sub:
                          kill_sets[&I].insert(ci);
                          break;
                        case cat_set:
                          kill_sets[&I].insert(ci);
                          break;
                      }
                    }
                  }
                }

                // remove self from kill set
                if (kill_sets[&I].find(&I) != kill_sets[&I].end()) kill_sets[&I].erase(kill_sets[&I].find(&I));
                break;
              }
              case cat_sub: {
                gen_sets[&I].insert(&I);

                Value* result = callInst->getArgOperand(0);
                if (isa<CallInst>(result)) {
                  kill_sets[&I].insert(cast<CallInst>(result));
                  kill_sets[cast<CallInst>(result)].insert(&I);
                }

                for (auto user : result->users()) {
                  if (isa<CallInst>(user)) {
                    CallInst *ci = cast<CallInst>(user);
                    if (result == ci->getArgOperand(0)) {
                      switch (funcToCatCode(ci->getCalledFunction()->getName())) {
                        case cat_add:
                          kill_sets[&I].insert(ci);
                          break;
                        case cat_sub:
                          kill_sets[&I].insert(ci);
                          break;
                        case cat_set:
                          kill_sets[&I].insert(ci);
                          break;
                      }
                    }
                  }
                }

                // remove self from kill set
                if (kill_sets[&I].find(&I) != kill_sets[&I].end()) kill_sets[&I].erase(kill_sets[&I].find(&I));
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
                gen_sets[&I].insert(&I);

                Value* result = callInst->getArgOperand(0);
                if (isa<CallInst>(result)) {
                  kill_sets[&I].insert(cast<CallInst>(result));
                  kill_sets[cast<CallInst>(result)].insert(&I);
                }

                for (auto user : result->users()) {
                  if (isa<CallInst>(user)) {
                    CallInst *ci = cast<CallInst>(user);
                    if (result == ci->getArgOperand(0)) {
                      switch (funcToCatCode(ci->getCalledFunction()->getName())) {
                        case cat_add:
                          kill_sets[&I].insert(ci);
                          break;
                        case cat_sub:
                          kill_sets[&I].insert(ci);
                          break;
                        case cat_set:
                          kill_sets[&I].insert(ci);
                          break;
                      }
                    }
                  }
                }

                // remove self from kill set
                if (kill_sets[&I].find(&I) != kill_sets[&I].end()) kill_sets[&I].erase(kill_sets[&I].find(&I));
                break;
              }
              case none: {
                // not a CAT function
                break;
              }
            }
          }
        }
      }

      // Build in and out sets from gen and kill sets
      bool outChanged;
      do {
        outChanged = false;

        for (auto &B : F) {
          if (DT.dominates(&*B.begin(), &B)) continue;

          // IN[i] = U OUT[p]
          std::set<Instruction *> pred_in = {};
          for (BasicBlock *pB : predecessors(&B)) {
            Instruction* terminator = pB->getTerminator();

            if (out_sets.find(terminator) != out_sets.end()) {
              for (Instruction* inst : out_sets[terminator]) {
                in_sets[&*B.begin()].insert(inst);
              }
            }
          }
          in_sets[&B.front()] = pred_in;

          for (auto& I : B) {
            // IN[i] - KILL[i]
            const std::set<Instruction *> set_diff = {};
            std::set_difference(in_sets[&I].begin(), in_sets[&I].end(), kill_sets[&I].begin(), kill_sets[&I].end(), set_diff.begin());

            // OUT[i] = GEN[i] U (IN[i] - KILL[i])
            const std::set<Instruction *> new_out_set = {};
            std::set_union(gen_sets[&I].begin(), gen_sets[&I].end(), set_diff.begin(), set_diff.end(), new_out_set.begin());

            // check for changes to the out set
            std::set<Instruction *> differences = {};
            std::set_difference(out_sets[&I].begin(), out_sets[&I].end(), new_out_set.begin(), new_out_set.end(), differences.begin());
            outChanged = outChanged || differences.size() > 0;
          }
        }
      } while (outChanged);

      printH3(F, in_sets, out_sets);

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
