import { createStructuredSelector } from 'reselect'

export const eventsSelector = createStructuredSelector({
    events: state => state.events.events,
    cached: state => state.events.cached,
    error: state => state.events.error,
    loading: state => state.events.loading,
    refreshing: state => state.events.refreshing
});