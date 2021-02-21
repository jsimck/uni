require('es6-promise').polyfill();
import fetch from 'isomorphic-fetch'
import jsCookie from 'js-cookie'
import { pushNotification } from '../../../actions/notifications'
import { closeModal } from '../actions/editModal'

// Add event actions
export const ADD_EVENT_REQUEST = 'ADD_EVENT_REQUEST';
export const ADD_EVENT_SUCCESS = 'ADD_EVENT_SUCCESS';
export const ADD_EVENT_ERROR = 'ADD_EVENT_ERROR';

export function addEventError() {
    return {
        type: ADD_EVENT_ERROR
    }
}

export function addEventRequest() {
    return {
        type: ADD_EVENT_REQUEST
    }
}

export function addEventSuccess(event) {
    return {
        type: ADD_EVENT_SUCCESS,
        event: event
    }
}

export function addEvent(title, description, content, startDate, endDate, mapId) {
    return function (dispatch) {
        dispatch(addEventRequest());

        return fetch('/events/', {
                method: 'POST',
                credentials: 'include',
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': jsCookie.get('csrftoken')
                },
                body: JSON.stringify({
                    title: title,
                    description: description,
                    content: content,
                    startDate: startDate,
                    endDate: endDate,
                    mapId: mapId
                })
            })
            .then(response => {
                if (response.status === 404) {
                    throw new Error('Chyba vytvoření události');
                }

                return response.json();
            })
            .then(json => {
                if (!json.success) {
                    throw new Error(json.message);
                }

                var event = json.data;

                dispatch(closeModal());
                dispatch(addEventSuccess(event));
                dispatch(pushNotification('success', 'Událost byla úspěšně vytvořena'));
            })
            .catch(error => {
                dispatch(addEventError());
                dispatch(pushNotification('danger', error.toString()));
            });
    }
}


// Remove event actions
export const REMOVE_EVENT_REQUEST = 'REMOVE_EVENT_REQUEST';
export const REMOVE_EVENT_SUCCESS = 'REMOVE_EVENT_SUCCESS';
export const REMOVE_EVENT_ERROR = 'REMOVE_EVENT_ERROR';

export function removeEventRequest() {
    return {
        type: REMOVE_EVENT_REQUEST
    }
}

export function removeEventError() {
    return {
        type: REMOVE_EVENT_ERROR
    }
}

export function removeEventSuccess(id) {
    return {
        type: REMOVE_EVENT_SUCCESS,
        id: id
    }
}

// Remove event actions
export function removeEvent(id) {
    return function(dispatch) {
        dispatch(removeEventRequest());

        return fetch('/events/' + id, {
                method: 'DELETE',
                credentials: 'include',
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': jsCookie.get('csrftoken')
                }
            })
            .then(response => {
                if (response.status === 404) {
                    throw new Error('Chyba smazání události');
                }

                return response.json();
            })
            .then(json => {
                if (!json.success) {
                    throw new Error(json.error);
                }

                dispatch(removeEventSuccess(id));
                dispatch(pushNotification('success', 'Událost byla úspěšně smazána'));
            })
            .catch(error => {
                dispatch(removeEventError());
                dispatch(pushNotification('danger', error.toString()));
            });
    }
}

// Add event actions
export const UPDATE_EVENT_REQUEST = 'UPDATE_EVENT_REQUEST';
export const UPDATE_EVENT_SUCCESS = 'UPDATE_EVENT_SUCCESS';
export const UPDATE_EVENT_ERROR = 'UPDATE_EVENT_ERROR';

export function updateEventError() {
    return {
        type: UPDATE_EVENT_ERROR
    }
}

export function updateEventRequest() {
    return {
        type: UPDATE_EVENT_REQUEST
    }
}

export function updateEventSuccess(updatedEvent) {
    return {
        type: UPDATE_EVENT_SUCCESS,
        event: updatedEvent
    }
}

export function updateEvent(event) {
    return function (dispatch) {
        dispatch(updateEventRequest());

        return fetch('/events/' + event.id, {
                method: 'PUT',
                credentials: 'include',
                headers: {
                    'Content-Type': 'application/json',
                    'X-CSRFToken': jsCookie.get('csrftoken')
                },
                body: JSON.stringify({
                    title: event.title,
                    description: event.description,
                    content: event.content,
                    startDate: event.startDate,
                    endDate: event.endDate,
                    mapId: event.mapId,
                    game: event.game || null
                })
            })
            .then(response => {
                if (response.status === 404) {
                    throw new Error('Chyba aktualizace události');
                }

                return response.json();
            })
            .then(json => {
                if (!json.success) {
                    throw new Error(json.message);
                }

                // Convert content to json
                var event = json.data;

                dispatch(closeModal());
                dispatch(updateEventSuccess(event));
                dispatch(pushNotification('success', 'Událost byla úspěšně aktualizována'));
            })
            .catch(error => {
                dispatch(updateEventError());
                dispatch(pushNotification('danger', error.toString()));
            });
    }
}

// Set Events action
export const SET_EVENTS = 'SET_EVENTS';

export function setEvents(events) {
    return {
        type: SET_EVENTS,
        events: events
    }
}