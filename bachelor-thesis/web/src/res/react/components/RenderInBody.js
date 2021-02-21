import React, { Component } from 'react'
import ReactDOM from 'react-dom'

class RenderInBody extends Component {
    componentDidMount() {
        this.container = document.createElement('div');
        document.body.appendChild(this.container);
        this.reRender();
    }

    componentDidUpdate() {
        this.reRender();
    }

    componentWillUnmount() {
        React.unmountComponentAtNode(this.container);
        document.body.removeChild(this.container);
    }

    reRender() {
        ReactDOM.render(this.props.children, this.container);
    }

    render() {
        return null;
    }
}

RenderInBody.propTypes = {
    children: React.PropTypes.any.isRequired
};

export default RenderInBody