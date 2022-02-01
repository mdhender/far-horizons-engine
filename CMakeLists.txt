cmake_minimum_required(VERSION 3.21)
project(far_horizons_engine C)
include_directories(engine/src)

set(CMAKE_C_STANDARD 99)

add_executable(fhcombat
        engine/src/fh.h
        engine/src/combat.h
        engine/src/Combat.c
        engine/src/combat_utils.c
        engine/src/utils.c
        engine/src/parse.c
        engine/src/for_jum.c
        engine/src/cons_op.c
        engine/src/do_germ.c
        engine/src/get_ship.c
        engine/src/fight_par.c
        engine/src/do_bat.c
        engine/src/do_round.c
        engine/src/do_bomb.c
        engine/src/do_siege.c
        engine/src/get_spnam.c
        engine/src/with_check.c
        engine/src/regen_sh.c
        engine/src/get_gal.c
        engine/src/get_plan.c
        engine/src/sav_plan.c
        engine/src/get_transact.c
        engine/src/sav_transact.c
        engine/src/with_check.h engine/src/do_round.h engine/src/for_jump.h engine/src/regen_sh.h engine/src/do_germ.h engine/src/do_bomb.h engine/src/do_siege.h)
