import React, { Component, AsyncStorage, UIManager } from 'react-native';
import devTools from 'remote-redux-devtools'
import { Provider } from 'react-redux';
import { createStore, applyMiddleware, compose } from 'redux';
import App from './layouts/Navigator';
import reducers from './reducers';
import { persistStore, autoRehydrate } from 'redux-persist'
import thunk from 'redux-thunk';

// Apply thunk middleware
const createStoreWithMiddleware = compose(
    applyMiddleware(thunk)
    // Connect devTools
    //devTools({
    //    hostname: '10.0.0.6',
    //    port: 7000
    //})
)(createStore);
const store = createStoreWithMiddleware(reducers, {}, autoRehydrate());

// Configure redux-persist
persistStore(store, {
    whitelist: ['events', 'games'],
    storage: AsyncStorage
});

// Enable layout animations on android
UIManager.setLayoutAnimationEnabledExperimental && UIManager.setLayoutAnimationEnabledExperimental(true);

class bc_mobile extends Component {
    render() {
        return (
            <Provider store={store}>
                <App />
            </Provider>
        );
    }
}

export default bc_mobile