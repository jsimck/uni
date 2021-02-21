import { createStructuredSelector } from 'reselect'

export const appSelector = createStructuredSelector({
    networkActivity: state => state.statusBar.networkActivity
});