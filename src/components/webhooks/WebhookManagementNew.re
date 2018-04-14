type state = {
  name: string,
  url: string,
  event: string,
};

type action =
  | ChangeName(string)
  | ChangeUrl(string)
  | ChangeEvent(string)
  | ClearInputs;

let component = ReasonReact.reducerComponent("WebhookManagementNew");

let make = (~create, _children) => {
  ...component,
  initialState: () => {name: "", url: "", event: ""},
  reducer: (action, state) =>
    switch (action) {
    | ChangeName(newVal) => ReasonReact.Update({...state, name: newVal})
    | ChangeUrl(newVal) => ReasonReact.Update({...state, url: newVal})
    | ChangeEvent(newVal) => ReasonReact.Update({...state, event: newVal})
    | ClearInputs => ReasonReact.Update({name: "", url: "", event: ""})
    },
  render: self => {
    let finishedEnteringData = () => {
      let newWebhook: Webhook.New.t = {
        name: self.state.name,
        url: self.state.url,
        event: self.state.event,
      };
      self.send(ClearInputs);
      create(newWebhook);
    };
    let getVal = ev => ReactDOMRe.domElementToObj(
                         ReactEventRe.Form.target(ev),
                       )##value;
    <tr>
      <td>
        <input
          value=self.state.name
          onChange=(ev => self.send(ChangeName(getVal(ev))))
        />
      </td>
      <td>
        <input
          value=self.state.url
          onChange=(ev => self.send(ChangeUrl(getVal(ev))))
        />
      </td>
      <td>
        <input
          value=self.state.event
          onChange=(ev => self.send(ChangeEvent(getVal(ev))))
        />
      </td>
      <td>
        <button onClick=((_) => finishedEnteringData())>
          (ReactUtils.s("Crear"))
        </button>
      </td>
    </tr>;
  },
};