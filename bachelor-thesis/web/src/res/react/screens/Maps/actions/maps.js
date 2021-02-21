require('es6-promise').polyfill();
import fetch from 'isomorphic-fetch'
import jsCookie from 'js-cookie'
import { hideMapsEditModal } from './mapsEditModal'
import { pushNotification } from '../../../actions/notifications'

// Add Map Actions
export const ADD_MAP_REQUEST = 'ADD_MAP_REQUEST';
export const ADD_MAP_SUCCESS = 'ADD_MAP_SUCCESS';
export const ADD_MAP_ERROR = 'ADD_MAP_ERROR';

export function addMapError() {
    return {
        type: ADD_MAP_ERROR
    };
}

export function addMapSuccess(map) {
    return {
        type: ADD_MAP_SUCCESS,
        map: map
    };
}

export function addMapRequest() {
    return {
        type: ADD_MAP_REQUEST
    };
}

export function addMap(title, description) {
    return dispatch => {
        dispatch(addMapRequest());

        return fetch('/maps/', {
                method: 'POST',
                credentials: 'include',
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': jsCookie.get('csrftoken')
                },
                body: JSON.stringify({
                    title: title,
                    description: description,
                    created: new Date(),
                    mapData: {}
                })
            })
            .then(response => {
                if (response.status == 404) {
                    throw new Error('Chyba vkládání mapy');
                }

                return response.json();
            })
            .then(json => {
                if (!json.success) {
                    throw new Error(json.message);
                }

                dispatch(addMapSuccess(json.data));
                dispatch(pushNotification('success', 'Mapa byla úspěšně vytvořena'));
                dispatch(hideMapsEditModal());
            })
            .catch(error => {
                dispatch(addMapError());
                dispatch(pushNotification('error', error.toString()));
            });
    };
}

// Remove Map Actions
export const REMOVE_MAP_REQUEST = 'REMOVE_MAP_REQUEST';
export const REMOVE_MAP_SUCCESS = 'REMOVE_MAP_SUCCESS';
export const REMOVE_MAP_ERROR = 'REMOVE_MAP_ERROR';

export function removeMapError() {
    return {
        type: REMOVE_MAP_ERROR
    };
}

export function removeMapSuccess(id) {
    return {
        type: REMOVE_MAP_SUCCESS,
        id: id
    };
}

export function removeMapRequest() {
    return {
        type: REMOVE_MAP_REQUEST
    };
}

export function removeMap(id) {
    return dispatch => {
        dispatch(removeMapRequest());

        return fetch('/maps/' + id, {
                method: 'DELETE',
                credentials: 'include',
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': jsCookie.get('csrftoken')
                }
            })
            .then(response => {
                if (response.status == 404) {
                    throw new Error('Chyba mazaní mapy');
                }

                return response.json();
            })
            .then(json => {
                if (!json.success) {
                    throw new Error(json.message);
                }

                dispatch(removeMapSuccess(id));
                dispatch(pushNotification('success', 'Mapa byla úspěšně smazána'));
            })
            .catch(error => {
                dispatch(removeMapError());
                dispatch(pushNotification('error', error.toString()));
            });
    };
}

// Edit Map Actions
export const EDIT_MAP_REQUEST = 'EDIT_MAP_REQUEST';
export const EDIT_MAP_SUCCESS = 'EDIT_MAP_SUCCESS';
export const EDIT_MAP_ERROR = 'EDIT_MAP_ERROR';

export function editMapError() {
    return {
        type: EDIT_MAP_ERROR
    };
}

export function editMapSuccess(map) {
    return {
        type: EDIT_MAP_SUCCESS,
        map: map
    };
}

export function editMapRequest() {
    return {
        type: EDIT_MAP_REQUEST
    };
}

export function editMap(map) {
    return (dispatch) => {
        dispatch(editMapRequest());

        return fetch('/maps/' + map.id, {
                method: 'PUT',
                credentials: 'include',
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': jsCookie.get('csrftoken')
                },
                body: JSON.stringify({
                    title: map.title,
                    description: map.description,
                    created: map.created,
                    updated: new Date(),
                    mapData: map.mapData
                })
            })
            .then(response => {
                if (response.status == 404) {
                    throw new Error('Chyba aktualizace mapy');
                }

                return response.json();
            })
            .then(json => {
                if (!json.success) {
                    throw new Error(json.message);
                }

                dispatch(editMapSuccess(json.data));
                dispatch(pushNotification('success', 'Událost byla úspěšně aktualizována'));
                dispatch(hideMapsEditModal());
            })
            .catch(error => {
                dispatch(editMapError());
                dispatch(pushNotification('error', error.toString()));
            });
    };
}

// Set Maps Actions
export const SET_MAPS = 'SET_MAPS';

export function setMaps(maps) {
    return {
        type: SET_MAPS,
        maps: maps
    }
}
