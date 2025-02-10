

#include "clang/StaticAnalyzer/Checkers/BuiltinCheckerRegistration.h"
#include "clang/StaticAnalyzer/Core/BugReporter/BugType.h"
#include "clang/StaticAnalyzer/Core/Checker.h"
#include "clang/StaticAnalyzer/Core/CheckerManager.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h"
#include "clang/StaticAnalyzer/Core/PathSensitive/ProgramStateTrait.h"

#include "clang/StaticAnalyzer/Core/PathSensitive/CallDescription.h"

using namespace clang;
using namespace ento;

#pragma clang optimize off

namespace {

// Since we are looking to extract the arguments, go with pre call for now
class ThreadModeling : public Checker<check::PreCall> {

  const CallDescriptionSet ThreadCreateCalls {
    { CDM::CLibrary, {"pthread_create"}, 4},
  };

  const FunctionDecl *GetFunctionDecl(SVal V, CheckerContext &C) const;
public:
  void checkPreCall(const CallEvent &Call, CheckerContext &C) const;
};

} // end anonymous namespace


void ThreadModeling::checkPreCall(const CallEvent &Call, CheckerContext &C) const {
  return;
#if 0
  if (!ThreadCreateCalls.contains(Call)) {
    return;
  }

  // 1. Get the `start_routine` argument
  ProgramStateRef State = C.getState();
  const FunctionDecl *CreateCall = reinterpret_cast<const FunctionDecl*>(Call.getDecl());

  // 2. Extract the start_routine parameter
  /* int pthread_create(pthread_t *restrict thread,
                          const pthread_attr_t *restrict attr,
                          void *(*start_routine)(void *),
                          void *restrict arg);
   */
  assert(Call.getNumArgs() == 4 && "pthread_create(3) should have 4 arguments");
  Expr const *StartRoutineExpr = Call.getArgExpr(2);
  assert(StartRoutineExpr && "start_routine should exist"); // XXX: might fail if in diff TU?

  // 3. Get the function pointer for `start_routine`
  SVal const SRV = C.getSVal(StartRoutineExpr);
  MemRegion const *SRR = SRV.getAsRegion();
  assert(SRR && "start_routine should be a pointer");

  // 4. Resolve FunctionDecl from pointer
  FunctionDecl const *StartRoutine = nullptr;

  if (auto const *FR = dyn_cast<FunctionCodeRegion>(SRR)) {
    StartRoutine = dyn_cast<FunctionDecl>(FR->getDecl());
  } // XXX: Can the function pointer be a different region type? (e.g. SymbolicRegion)
  assert(StartRoutine && "start_routine be a valid function pointer");

  // 5. Get AST (single TU for now)
  // 6. Resolve AST to Call
  // 7. Inline Call

#endif
}

const FunctionDecl *ThreadModeling::GetFunctionDecl(SVal V, CheckerContext &C) const {
  if (const FunctionDecl *FD = V.getAsFunctionDecl())
    return FD;
  return nullptr;
}

void clang::ento::registerThreadModeling(CheckerManager &Mgr) {
  Mgr.registerChecker<ThreadModeling>();
}

bool clang::ento::shouldRegisterThreadModeling(const CheckerManager &) {
  return true;
}
