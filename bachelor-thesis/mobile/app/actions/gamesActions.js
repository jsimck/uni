import API from '../services/APIService'

// Manage games actions
export const ADD_GAME = 'ADD_GAME';
export const UPDATE_GAME = 'UPDATE_GAME';

export function addGame(id, qrId) {
    return {
        type: ADD_GAME,
        id: id,
        qrId: qrId
    };
}

export function updateGame(game) {
    return {
        type: UPDATE_GAME,
        game: game
    };
}