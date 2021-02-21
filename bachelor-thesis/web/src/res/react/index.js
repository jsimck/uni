import React from 'react'
import { render } from 'react-dom'
import { createStore, applyMiddleware, compose } from 'redux'
import { Provider } from 'react-redux'
import thunk from 'redux-thunk'
import { Router, Route, Link, browserHistory } from 'react-router'
import EventsScreen from './screens/Events/index'
import MapsScreen from './screens/Maps'
import MapEditorScreen from './screens/MapEditor'
import LoginScreen from './screens/Login'
import QrGeneratorScreen from './screens/QrGenerator'
import reducers from './reducers/index'

const finalCreateStore = compose(
    applyMiddleware(thunk),
    window.devToolsExtension ? window.devToolsExtension() : f => f
)(createStore);
const store = finalCreateStore(
    reducers
);

render((
    <Provider store={store}>
        <Router history={browserHistory}>
            <Route path="/" component={LoginScreen} />
            <Route path="/qrgenerator" component={QrGeneratorScreen} />
            <Route path="/events" component={EventsScreen} />
            <Route path="/maps" component={MapsScreen} />
            <Route path="/maps/:mapId/editor" component={MapEditorScreen}/>
        </Router>
    </Provider>
), document.getElementById('react-root'));