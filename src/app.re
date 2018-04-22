[%bs.raw {|require('./app.css')|}];

/* var _LTracker = _LTracker || [];
   _LTracker.push({'logglyKey': '',
   'sendConsoleErrors' : true,
   'tag' : 'loggly-jslogger'  }); */
let component = ReasonReact.statelessComponent("App");

let make = _children => {
  ...component,
  render: _self => <div className="App"> <AppHeader /> <CafeRouter /> </div>,
};