import { combineReducers } from 'redux'

// Global Components
import notifications from './notifications'
import qrModal from './qrModal'

// Map Editor
import canvas from '../screens/MapEditor/reducers/canvas'
import canvasTool from '../screens/MapEditor/reducers/canvasTool'
import canvasKeyMap from '../screens/MapEditor/reducers/canvasKeyMap'
import canvasMouseEvent from '../screens/MapEditor/reducers/canvasMouseEvent'
import editActionModal from '../screens/MapEditor/reducers/editActionModal'

// Maps
import maps from '../screens/Maps/reducers/maps'
import mapsEditModal from '../screens/Maps/reducers/mapsEditModal'
import removeMapConfirm from '../screens/Maps/reducers/removeMapConfirm'

// Events
import events from '../screens/Events/reducers/events'
import editModal from '../screens/Events/reducers/editModal'
import removeEventConfirm from '../screens/Events/reducers/removeEventConfirm'
import eventsGameModal from '../screens/Events/reducers/eventsGameModal'

// Login
import login from '../screens/Login/reducers/loginReducer'

export default combineReducers({
    maps,
    mapsEditModal,
    canvas,
    canvasTool,
    canvasKeyMap,
    canvasMouseEvent,
    notifications,
    editActionModal,
    events,
    qrModal,
    editModal,
    removeEventConfirm,
    removeMapConfirm,
    eventsGameModal,
    login
})