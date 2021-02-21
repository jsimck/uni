import _ from 'lodash'
import * as actions from '../actions/gamesActions'
import { REHYDRATE } from 'redux-persist/constants'

const initialState = {
    games: []
};

const games = (state = initialState, action) => {
    switch (action.type) {
        case actions.ADD_GAME:
            return {
                ...state,
                games: [
                    ...state.games,
                    {
                        id: action.id,
                        qrIds: [action.qrId]
                    }
                ]
            };

        case actions.UPDATE_GAME:
            var index = _.findIndex(state.games, {id: action.game.id});

            return {
                ...state,
                games: [
                    ..._.slice(state.games, 0, index),
                    action.game,
                    ..._.slice(state.games, index + 1)
                ]
            };

        case REHYDRATE:
            if (!_.isUndefined(action.payload.games)) {
                return {
                    ...state,
                    games: [
                        ...action.payload.games.games
                    ]
                };
            }

            return state;

        default:
            return state;
    }
};

export default games