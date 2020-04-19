; ModuleID = 'program1.bc'
source_filename = "program1.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32, i8**) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i8**, align 8
  %6 = alloca i8*, align 8
  %7 = alloca i8*, align 8
  %8 = alloca i8*, align 8
  store i32 0, i32* %3, align 4
  store i32 %0, i32* %4, align 4
  store i8** %1, i8*** %5, align 8
  %9 = call i8* @CAT_new(i64 5)
  store i8* %9, i8** %6, align 8
  %10 = call i8* @CAT_new(i64 8)
  store i8* %10, i8** %7, align 8
  %11 = call i8* @CAT_new(i64 0)
  store i8* %11, i8** %8, align 8
  %12 = load i8*, i8** %8, align 8
  %13 = load i8*, i8** %6, align 8
  %14 = load i8*, i8** %7, align 8
  call void @CAT_add(i8* %12, i8* %13, i8* %14)
  %15 = load i8*, i8** %6, align 8
  call void @CAT_set(i8* %15, i64 3)
  %16 = load i8*, i8** %8, align 8
  %17 = call i64 @CAT_get(i8* %16)
  %18 = trunc i64 %17 to i32
  ret i32 %18

19:
  %20 = call i64 @CAT_get(i8* %16)
  %21 = trunc i64 %20 to i32
  ret i32 %21
}

declare dso_local i8* @CAT_new(i64) #1

declare dso_local void @CAT_add(i8*, i8*, i8*) #1

declare dso_local void @CAT_set(i8*, i64) #1

declare dso_local i64 @CAT_get(i8*) #1

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 9.0.0 (git@github.com:scampanoni/LLVM_installer.git 05e8bdd81a8fb30a2498881405757dbddd5535d6)"}
