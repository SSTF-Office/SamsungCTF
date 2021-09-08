import React from 'react';

const Spinner = () => (
        <div className="container" id="horizon-cell">
        <div id="horizon">
        <div className="preloader-wrapper big active">
        <div className="spinner-layer spinner-blue">
        <div className="circle-clipper left">
        <div className="circle"></div>
        </div><div className="gap-patch">
        <div className="circle"></div>
        </div><div className="circle-clipper right">
        <div className="circle"></div>
        </div>
        </div>

        <div className="spinner-layer spinner-red">
        <div className="circle-clipper left">
        <div className="circle"></div>
        </div><div className="gap-patch">
        <div className="circle"></div>
        </div><div className="circle-clipper right">
        <div className="circle"></div>
        </div>
        </div>

        <div className="spinner-layer spinner-yellow">
        <div className="circle-clipper left">
        <div className="circle"></div>
        </div><div className="gap-patch">
        <div className="circle"></div>
        </div><div className="circle-clipper right">
        <div className="circle"></div>
        </div>
        </div>

        <div className="spinner-layer spinner-green">
        <div className="circle-clipper left">
        <div className="circle"></div>
        </div><div className="gap-patch">
        <div className="circle"></div>
        </div><div className="circle-clipper right">
        <div className="circle"></div>
        </div>
        </div>
        </div>
        <div className="row">
        <h5 className="black-text"> Loading... </h5>
        </div>
        </div>
        </div>
)

export default Spinner;
