#pragma once

#include "Element/Parameter.h"
#include "Element/Property.h"
#include "Element/Variable.h"

#include "Helper/AccessHelper.h"
#include "Helper/InstanceHelper.h"
#include "Helper/TypeHelper.h"

#include "Relation/AggregationEnd.h"
#include "Relation/AssociationEnd.h"
#include "Relation/ReferEnd.h"
#include "Relation/SharedEnd.h"
#include "Relation/UniqueEnd.h"

#include "Relation/Tool/Cpp/Raw/ReferEnd.h"
#include "Relation/Tool/Std/Shared/ReferEnd.h"
#include "Relation/Tool/Std/Shared/SharedEnd.h"
#include "Relation/Tool/Std/Shared/UniqueEnd.h"
//#include "Tool/Std/Shared/Conversion.h"

#include "Value/Instance.h"

#include "Value/Tool/Cpp/Inplace/InplaceTool.h"
#include "Value/Tool/Cpp/Raw/HeapTool.h"
#include "Value/Tool/Std/Mutex/AtomicTool.h"
#include "Value/Tool/Std/Shared/ImplicitTool.h"
#include "Value/Tool/Std/Shared/HeapTool.h"
#include "Value/Tool/Std/Unique/HeapTool.h"

