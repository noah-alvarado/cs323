; ModuleID = 'CAT.bc'
source_filename = "CAT.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%struct._IO_FILE = type { i32, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, i8*, %struct._IO_marker*, %struct._IO_FILE*, i32, i32, i64, i16, i8, [1 x i8], i8*, i64, i8*, i8*, i8*, i8*, i64, i32, [20 x i8] }
%struct._IO_marker = type { %struct._IO_marker*, %struct._IO_FILE*, i32 }
%struct.internal_data_t = type { [20 x i8], i64, [20 x i8] }

@invocations = internal unnamed_addr global i64 0, align 8
@.str = private unnamed_addr constant [21 x i8] c"p6pbbUlpLo0BL1bM2k8K\00", align 1
@stderr = external dso_local local_unnamed_addr global %struct._IO_FILE*, align 8
@.str.1 = private unnamed_addr constant [31 x i8] c"libCAT: ERROR = input is NULL\0A\00", align 1
@.str.2 = private unnamed_addr constant [41 x i8] c"libCAT: ERROR = data has been corrupted\0A\00", align 1

; Function Attrs: nounwind uwtable
define dso_local noalias i8* @CAT_new(i64) local_unnamed_addr #0 {
  %2 = load i64, i64* @invocations, align 8, !tbaa !2
  %3 = add nsw i64 %2, 1
  store i64 %3, i64* @invocations, align 8, !tbaa !2
  %4 = tail call noalias i8* @malloc(i64 56) #6
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %4, i8* align 1 getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 20, i1 false)
  %5 = getelementptr inbounds i8, i8* %4, i64 32
  tail call void @llvm.memcpy.p0i8.p0i8.i64(i8* nonnull align 1 %5, i8* align 1 getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 20, i1 false)
  %6 = getelementptr inbounds i8, i8* %4, i64 24
  %7 = bitcast i8* %6 to i64*
  store i64 %0, i64* %7, align 8, !tbaa !6
  ret i8* %4
}

; Function Attrs: nounwind
declare dso_local noalias i8* @malloc(i64) local_unnamed_addr #1

; Function Attrs: nounwind uwtable
define dso_local i64 @CAT_get(i8* readonly) local_unnamed_addr #0 {
  %2 = load i64, i64* @invocations, align 8, !tbaa !2
  %3 = add nsw i64 %2, 1
  store i64 %3, i64* @invocations, align 8, !tbaa !2
  %4 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %0)
  %5 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %4, i64 0, i32 1
  %6 = load i64, i64* %5, align 8, !tbaa !6
  ret i64 %6
}

; Function Attrs: nounwind uwtable
define internal fastcc %struct.internal_data_t* @internal_check_data(i8* readonly) unnamed_addr #0 {
  %2 = icmp eq i8* %0, null
  br i1 %2, label %3, label %6

; <label>:3:                                      ; preds = %1
  %4 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !tbaa !8
  %5 = tail call i64 @fwrite(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @.str.1, i64 0, i64 0), i64 30, i64 1, %struct._IO_FILE* %4) #7
  tail call void @abort() #8
  unreachable

; <label>:6:                                      ; preds = %1
  %7 = bitcast i8* %0 to %struct.internal_data_t*
  %8 = tail call i32 @strncmp(i8* nonnull %0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 20) #9
  %9 = icmp eq i32 %8, 0
  br i1 %9, label %10, label %14

; <label>:10:                                     ; preds = %6
  %11 = getelementptr inbounds i8, i8* %0, i64 32
  %12 = tail call i32 @strncmp(i8* nonnull %11, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i64 0, i64 0), i64 20) #9
  %13 = icmp eq i32 %12, 0
  br i1 %13, label %17, label %14

; <label>:14:                                     ; preds = %10, %6
  %15 = load %struct._IO_FILE*, %struct._IO_FILE** @stderr, align 8, !tbaa !8
  %16 = tail call i64 @fwrite(i8* getelementptr inbounds ([41 x i8], [41 x i8]* @.str.2, i64 0, i64 0), i64 40, i64 1, %struct._IO_FILE* %15) #7
  tail call void @abort() #8
  unreachable

; <label>:17:                                     ; preds = %10
  ret %struct.internal_data_t* %7
}

; Function Attrs: nounwind uwtable
define dso_local void @CAT_set(i8*, i64) local_unnamed_addr #0 {
  %3 = load i64, i64* @invocations, align 8, !tbaa !2
  %4 = add nsw i64 %3, 1
  store i64 %4, i64* @invocations, align 8, !tbaa !2
  %5 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %0)
  %6 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %5, i64 0, i32 1
  store i64 %1, i64* %6, align 8, !tbaa !6
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @CAT_sub(i8*, i8* readonly, i8* readonly) local_unnamed_addr #0 {
  %4 = load i64, i64* @invocations, align 8, !tbaa !2
  %5 = add nsw i64 %4, 1
  store i64 %5, i64* @invocations, align 8, !tbaa !2
  %6 = icmp eq i8* %1, null
  %7 = icmp eq i8* %2, null
  %8 = or i1 %6, %7
  br i1 %8, label %19, label %9

; <label>:9:                                      ; preds = %3
  %10 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* nonnull %1)
  %11 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* nonnull %2)
  %12 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %0)
  %13 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %10, i64 0, i32 1
  %14 = load i64, i64* %13, align 8, !tbaa !6
  %15 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %11, i64 0, i32 1
  %16 = load i64, i64* %15, align 8, !tbaa !6
  %17 = sub nsw i64 %14, %16
  %18 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %12, i64 0, i32 1
  store i64 %17, i64* %18, align 8, !tbaa !6
  br label %19

; <label>:19:                                     ; preds = %3, %9
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @CAT_add(i8*, i8* readonly, i8* readonly) local_unnamed_addr #0 {
  %4 = load i64, i64* @invocations, align 8, !tbaa !2
  %5 = add nsw i64 %4, 1
  store i64 %5, i64* @invocations, align 8, !tbaa !2
  %6 = icmp eq i8* %1, null
  %7 = icmp eq i8* %2, null
  %8 = or i1 %6, %7
  br i1 %8, label %19, label %9

; <label>:9:                                      ; preds = %3
  %10 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* nonnull %1)
  %11 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* nonnull %2)
  %12 = tail call fastcc %struct.internal_data_t* @internal_check_data(i8* %0)
  %13 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %10, i64 0, i32 1
  %14 = load i64, i64* %13, align 8, !tbaa !6
  %15 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %11, i64 0, i32 1
  %16 = load i64, i64* %15, align 8, !tbaa !6
  %17 = add nsw i64 %16, %14
  %18 = getelementptr inbounds %struct.internal_data_t, %struct.internal_data_t* %12, i64 0, i32 1
  store i64 %17, i64* %18, align 8, !tbaa !6
  br label %19

; <label>:19:                                     ; preds = %3, %9
  ret void
}

; Function Attrs: norecurse nounwind readonly uwtable
define dso_local i64 @CAT_invocations() local_unnamed_addr #2 {
  %1 = load i64, i64* @invocations, align 8, !tbaa !2
  ret i64 %1
}

; Function Attrs: noreturn nounwind
declare dso_local void @abort() local_unnamed_addr #3

; Function Attrs: nounwind readonly
declare dso_local i32 @strncmp(i8* nocapture, i8* nocapture, i64) local_unnamed_addr #4

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i1) #5

; Function Attrs: nounwind
declare i64 @fwrite(i8* nocapture, i64, i64, %struct._IO_FILE* nocapture) local_unnamed_addr #6

attributes #0 = { nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { norecurse nounwind readonly uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { noreturn nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { argmemonly nounwind }
attributes #6 = { nounwind }
attributes #7 = { cold }
attributes #8 = { noreturn nounwind }
attributes #9 = { nounwind readonly }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
!2 = !{!3, !3, i64 0}
!3 = !{!"long", !4, i64 0}
!4 = !{!"omnipotent char", !5, i64 0}
!5 = !{!"Simple C/C++ TBAA"}
!6 = !{!7, !3, i64 24}
!7 = !{!"", !4, i64 0, !3, i64 24, !4, i64 32}
!8 = !{!9, !9, i64 0}
!9 = !{!"any pointer", !4, i64 0}
