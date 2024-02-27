#pragma once


enum class ER_ITEM_CODE
{
	SCISSORS,				// 가위	
	LEATHER,				// 가죽	
	TURTLE_SHELL,			// 거북이 등딱지		
	RUBBER,					// 고무
	SCRAP_METAL,			// 고철		
	PICKAXE,				// 곡괭이	
	FEATHER,				// 깃털	
	FLOWER,					// 꽃
	BRANCH,					// 나뭇가지
	RUSTY_SWORD,			// 녹슨 검		
	SHORT_ROD,				// 단봉	
	STONE,					// 돌맹이
	LIGHTER,				// 라이터	
	RIBBON,					// 리본
	STALLION_MEDAL,			// 마패		
	FOUNTAIN_PEN,			// 만년필		
	HAMMER,					// 망치
	HAIRBAND,				// 머리띠	
	COTTON_GLOVES,			// 목장갑		
	NAIL,					// 못
	WALTHER_PPK,			// 발터 PPK		
	BATTERY,				// 배터리	
	BANDAGE,				// 붕대	
	HOLY_GRAIL,				// 성배	
	SHIRT,					// 셔츠
	WATCH,					// 손목시계
	STEEL_CHAIN,			// 쇠사슬		
	CROSS,					// 십자가
	BOW,					// 양궁
	OIL,					// 오일
	CLOTH,					// 옷감
	RUNNING_SHOES,			// 운동화		
	GEMSTONE,				// 원석	
	GLASS_BOTTLE,			// 유리병		
	HELMET,					// 자전거 헬멧
	GLUE,					// 접착제
	PAPER,					// 종이
	SPARKLING_WATER,		// 탄산수			
	TRUMP_CARD,				// 트럼프 카드	
	BRACELET,				// 팔찌	
	PIANO_WIRE,				// 피아노선	
	GUNPOWDER,				// 화약	
	LEATHER_SHIELD,			// 가죽 방패		
	BROKEN_WATCH,			// 고장난 시계		
	MILITARY_SUIT,			// 군복		
	OILCLOTH,				// 기름먹인 천	
	REPAIRED_SLIPPERS,		// 덧댄 슬리퍼			
	RUBY,					// 루비
	MAGNUM_PYTHON,			// 매그넘-파이선		
	CIRCLET,				// 머리테	
	FEATHER_DUSTER,			// 먼지털이개		
	BOOTS,					// 부츠
	SHAMANS_BRONZE,			// 비파단도		
	CHAIN_SCYTHE,			// 사슬 낫		
	DEACON_ROBES,			// 사제복		
	LONG_SWORD,				// 장검	
	SAINTS_RELIC,			// 성자의 유산		
	IRON_KNUCKLES,			// 아이언 너클		
	SAFETY_HELMET,			// 안전모		
	FLOWER_OF_FATE,			// 운명의 꽃		
	GLASS_PIECES,			// 유리 조각		
	ION_BATTERY,			// 이온 전지		
	DIVING_SUIT,			// 잠수복		
	LONGBOW,				// 장궁	
	ASH,					// 재
	ELECTRONIC_PARTS,		// 전자 부품			
	BLUEPRINT,				// 정교한 도면	
	IRON_SHEET,				// 철판	
	HIGH_HEELS,				// 하이힐	
	GOLD,					// 황금
	STEEL_SHIELD,			// 강철 방패		
	GAUNTLET,				// 건틀릿	
	GOLDEN_BRACELET,		// 금팔찌			
	MAGNUM_ANACONDA,		// 매그넘-아나콘다			
	MOTOR,					// 모터
	BALLISTIC_HELMET,		// 방탄모			
	BULLETPROOF_VEST,		// 방탄조끼			
	MIGHTY_BOW,				// 벽력궁	
	JEWEL_SWORD,			// 보검		
	REAPERS_SCYTHE,			// 사신의낫		
	AETHER_FRINGE,			// 아이테르 깃털		
	GLASS_PANEL,			// 유리판		
	COMBAT_BOOTS,			// 전투화		
	SUIT,					// 정장
	TIARA,					// 티아라
	SHEET_METAL_ARMOR,		// 판금 갑옷			
	FEATHER_BOOTS,			// 경량화 부츠		
	OPTICAL_CAMOUFLAGE_SUIT,// 광학미채 슈트					
	VIRTUOUS_OUTLAW,		// 로빈			
	MAGNUM_BOA,				// 매그넘-보아	
	VITAL_SIGN_SENSOR,		// 바이탈 센서			
	TUXEDO,					// 집사복
	BUCEPHALUS,				// 부케팔로스	
	BRASIL_GAUNTLET,		// 브레이질 건틀릿			
	CRUSADER_ARMOR,			// 성기사의 갑옷		
	CRYSTAL_TIARA,			// 수정 티아라		
	EXCALIBUR,				// 엑스칼리버	
	MOTORCYCLE_HELMET,		// 오토바이 헬멧			
	TACTICAL_OPS_HELMET,	// 전술 - ops 헬멧				
	JEBES_ALTERED_BOW,		// 제배의 활			
	HIGH_PRIEST_ROBES,		// 제사장의 예복			
	WHITE_RHINOS,			// 클링온 부츠		
	TINDALOS_BAND,			// 탄달로스의 팔찌		
	STRAITJACKET_SNEAKERS,	// 풍화륜				
	PLASMA_ARC,				// 플라즈마 아크	
	HARPE,					// 하르페
	EOD_SUIT,				// EOD 슈트
	END,
};

enum class ER_ITEM_GRADE
{
	NONE,
	COMMON,		// 일반
	UNCOMMONE,	// 고급
	RARE,		// 희귀
	EPIC,		// 영웅
};

enum class ER_ITEM_TYPE
{
	NONE,
	EQUIPMENT,			// 장비
	CONSUMABLES,		// 소비
	INGREDIENT,			// 재료
};

enum class ER_ITEM_SLOT
{
	NONE,				// 장착불가
	WEAPONS,			// 무기
	HEAD,				// 머리
	CHEST,				// 옷
	ARM,				// 팔
	LEG,				// 다리
	END

};


typedef union item_combination_pair
{
	struct
	{
		UINT ingredient_1;
		UINT ingredient_2;
	};

	DWORD_PTR recipe;
}ER_RECIPE;

extern const wchar_t* ER_ITEM_WSTR[(UINT)ER_ITEM_CODE::END];