// -*- mode: c; tab-width: 4; indent-tabs-mode: nil; st-rulers: [132] -*-
// vim: ts=4 sw=4 ft=c et

#include "nativelists_nif.h"
#include "xnif_slice.h"

#include <unistd.h>

ErlNifMutex *nativelists_nif_mutex = NULL;

static ERL_NIF_TERM ATOM_badarg;
static ERL_NIF_TERM ATOM_error;
static ERL_NIF_TERM ATOM_false;
static ERL_NIF_TERM ATOM_nil;
static ERL_NIF_TERM ATOM_ok;
static ERL_NIF_TERM ATOM_true;
static ERL_NIF_TERM ATOM_undefined;

/* NIF Function Declarations */

static ERL_NIF_TERM nativelists_nif_keydelete_3(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

/* NIF Function Definitions */

/* nativelists_nif:keydelete/3 */

// static int
// nativelists_keymember(ErlNifEnv *env, ERL_NIF_TERM key, unsigned int n, ERL_NIF_TERM tuple_list)
// {
//     ERL_NIF_TERM head;
//     ERL_NIF_TERM tail;
//     int arity = 0;
//     const ERL_NIF_TERM *tuple = NULL;

//     tail = tuple_list;
//     while (enif_get_list_cell(env, tail, &head, &tail)) {
//         if (enif_get_tuple(env, head, &arity, &tuple) && arity > 0 && (arity - 1) >= n && enif_compare(tuple[n], key) == 0) {
//             return 1;
//         }
//     }

//     return 0;
// }

static ERL_NIF_TERM
nativelists_nif_keydelete_3(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    ERL_NIF_TERM key;
    ERL_NIF_TERM list;
    unsigned int n;
    size_t length;
    ERL_NIF_TERM head;
    ERL_NIF_TERM tail;
    int arity = 0;
    const ERL_NIF_TERM *tuple = NULL;
    // ERL_NIF_TERM *array = NULL;
    size_t index = 0;
    ERL_NIF_TERM heads;

    if (argc != 3 || !enif_get_uint(env, argv[1], &n) || n == 0 || !enif_is_list(env, argv[2]) || !enif_get_list_length(env, argv[2], (unsigned *)&length)) {
        return enif_make_badarg(env);
    }

    key = argv[0];
    n--;
    list = argv[2];
    tail = list;

    if (length == 0) {
        return list;
    }

    // if (!nativelists_keymember(env, key, n, list)) {
    //     return list;
    // }

    heads = enif_make_list(env, 0);
    while (enif_get_list_cell(env, tail, &head, &tail)) {
        if (enif_get_tuple(env, head, &arity, &tuple) && arity > 0 && (arity - 1) >= n && enif_compare(tuple[n], key) == 0) {
            continue;
        } else {
            heads = enif_make_list_cell(env, head, heads);
            index++;
        }
    }

    if (index == length) {
        return list;
    }

    return heads;

    // array = (void *)enif_alloc(sizeof(ERL_NIF_TERM *) * length);
    // if (array == NULL) {
    //     return enif_make_badarg(env);
    // }

    // while (enif_get_list_cell(env, tail, &head, &tail)) {
    //     if (enif_get_tuple(env, head, &arity, &tuple) && arity > 0 && (arity - 1) >= n && enif_compare(tuple[n], key) == 0) {
    //         continue;
    //     } else {
    //         array[index] = head;
    //         index++;
    //     }
    // }

    // if (index == length) {
    //     (void)enif_free((void *)array);
    //     return list;
    // }

    // list = enif_make_list_from_array(env, array, index);
    // (void)enif_free((void *)array);
    // return list;
}

/* NIF Callbacks */

static ErlNifFunc nativelists_nif_funcs[] = {{"keydelete", 3, nativelists_nif_keydelete_3, ERL_NIF_DIRTY_JOB_CPU_BOUND}};

static void nativelists_nif_make_atoms(ErlNifEnv *env);
static int nativelists_nif_load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info);
static int nativelists_nif_upgrade(ErlNifEnv *env, void **priv_data, void **old_priv_data, ERL_NIF_TERM load_info);
static void nativelists_nif_unload(ErlNifEnv *env, void *priv_data);

static void
nativelists_nif_make_atoms(ErlNifEnv *env)
{
#define ATOM(Id, Value)                                                                                                            \
    {                                                                                                                              \
        Id = enif_make_atom(env, Value);                                                                                           \
    }
    ATOM(ATOM_badarg, "badarg");
    ATOM(ATOM_error, "error");
    ATOM(ATOM_false, "false");
    ATOM(ATOM_nil, "nil");
    ATOM(ATOM_ok, "ok");
    ATOM(ATOM_true, "true");
    ATOM(ATOM_undefined, "undefined");
#undef ATOM
}

static int
nativelists_nif_load(ErlNifEnv *env, void **priv_data, ERL_NIF_TERM load_info)
{
    int retval;

    if (nativelists_nif_mutex == NULL) {
        nativelists_nif_mutex = enif_mutex_create("nativelists_nif_mutex");
    }
    (void)enif_mutex_lock(nativelists_nif_mutex);

    /* Load xnif_slice */
    if ((retval = xnif_slice_load(env, priv_data, load_info)) != 0) {
        (void)enif_mutex_unlock(nativelists_nif_mutex);
        return retval;
    }

    /* Initialize common atoms */
    (void)nativelists_nif_make_atoms(env);

    (void)enif_mutex_unlock(nativelists_nif_mutex);
    return retval;
}

static int
nativelists_nif_upgrade(ErlNifEnv *env, void **priv_data, void **old_priv_data, ERL_NIF_TERM load_info)
{
    int retval;

    if (nativelists_nif_mutex == NULL) {
        nativelists_nif_mutex = enif_mutex_create("nativelists_nif_mutex");
    }
    (void)enif_mutex_lock(nativelists_nif_mutex);

    /* Upgrade xnif_slice */
    if ((retval = xnif_slice_upgrade(env, priv_data, old_priv_data, load_info)) != 0) {
        (void)enif_mutex_unlock(nativelists_nif_mutex);
        return retval;
    }

    /* Initialize common atoms */
    (void)nativelists_nif_make_atoms(env);

    (void)enif_mutex_unlock(nativelists_nif_mutex);
    return retval;
}

static void
nativelists_nif_unload(ErlNifEnv *env, void *priv_data)
{
    if (nativelists_nif_mutex != NULL) {
        (void)enif_mutex_lock(nativelists_nif_mutex);
    }
    (void)xnif_slice_unload(env, priv_data);
    if (nativelists_nif_mutex != NULL) {
        (void)enif_mutex_unlock(nativelists_nif_mutex);
        (void)enif_mutex_destroy(nativelists_nif_mutex);
        nativelists_nif_mutex = NULL;
    }
    return;
}

ERL_NIF_INIT(nativelists_nif, nativelists_nif_funcs, nativelists_nif_load, NULL, nativelists_nif_upgrade, nativelists_nif_unload);
