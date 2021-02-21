import { combineReducers } from 'redux'

// Reducers
import events from './eventsReducer'
import action from './actionReducer'
import statusBar from './statusBarReducer'
import notificationAlert from './notificationAlertReducer'
import qrReader from './qrReaderReducer'
import gamesModal from './gameModalReducer'
import games from './gamesReducer'

export default combineReducers({
    events,
    action,
    statusBar,
    notificationAlert,
    gamesModal,
    games,
    qrReader
})