################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
"../Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.c" 

COMPILED_SRCS += \
"Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.src" 

C_DEPS += \
"./Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.d" 

OBJS += \
"Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.o" 


# Each subdirectory must supply rules for building sources it contributes
"Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.src":"../Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.c" "Libraries/iLLD/TC37A/Tricore/Ccu6/Std/subdir.mk"
	cctc -cs --dep-file="$*.d" --misrac-version=2004 -D__CPU__=tc37x "-fZ:/home/mr_monopoly/Alberto/programmazione/Cproject/raceup/ControlUnitLogicOperator/instances/aurix_375_lte/TriCore Release (TASKING)/TASKING_C_C___Compiler-Include_paths__-I_.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc37x -Y0 -N0 -Z0 -o "$@" "$<"
"Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.o":"Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.src" "Libraries/iLLD/TC37A/Tricore/Ccu6/Std/subdir.mk"
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<"

clean: clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Ccu6-2f-Std

clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Ccu6-2f-Std:
	-$(RM) ./Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.d ./Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.o ./Libraries/iLLD/TC37A/Tricore/Ccu6/Std/IfxCcu6.src

.PHONY: clean-Libraries-2f-iLLD-2f-TC37A-2f-Tricore-2f-Ccu6-2f-Std
