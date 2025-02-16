@echo off

set start_flag=/low
set tex3ds_path=C:\devkitPro\tools\bin\tex3ds
set compress_flag=-z auto
set pixel_format_flag=-f a4

mkdir _build
mkdir _preview

start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i basic_latin\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i latin_1_supplement\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i latin_extended-a\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i latin_extended-b\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i ipa_extensions\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i spacing_modifier_letters\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i combining_diacritical_marks\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i greek_and_coptic\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cyrillic\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cyrillic_supplement\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i armenian\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hebrew\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i arabic\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i devanagari\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i gurmukhi\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i tamil\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i telugu\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i kannada\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i sinhala\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i thai\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i lao\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i tibetan\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i georgian\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i unified_canadian_aboriginal_syllabics\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i phonetic_extensions\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i combining_diacritical_marks_supplement\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i greek_extended\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i general_punctuation\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i superscripts_and_subscripts\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i combining_diacritical_marks_for_symbols\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i arrows\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i mathematical_operators\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i miscellaneous_technical\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i optical_character_recognition\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i enclosed_alphanumerics\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i box_drawing\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i block_elements\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i geometric_shapes\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i miscellaneous_symbols\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i dingbats\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i supplemental_arrows_b\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i miscellaneous_symbols_and_arrows\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_symbol_and_punctuation\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hiragana\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i katakana\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_compatibility\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_0.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_1.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_2.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_3.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_4.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_5.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_6.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_7.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_8.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_9.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_10.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_11.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_12.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_13.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_14.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_15.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_16.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_17.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_18.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_19.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_unified_ideographs\options_20.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i yi_syllables\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i yi_radicals\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_0.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_1.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_2.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_3.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_4.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_5.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_6.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_7.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_8.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_9.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i hangul_syllables\options_10.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i cjk_compatibility_forms\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i halfwidth_and_fullwidth_forms\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i miscellaneous_symbols_and_pictographs\options.txt
start %start_flag% %tex3ds_path% --atlas %pixel_format_flag% %compress_flag% -i emoticons\options.txt

