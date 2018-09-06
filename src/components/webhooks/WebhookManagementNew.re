open ReactUtils;

type state = {
  name: string,
  url: string,
  event: string,
  source: string,
  behavior: string,
  payload: string,
};

type action =
  | ChangeName(string)
  | ChangeUrl(string)
  | ChangeEvent(string)
  | ChangeSource(string)
  | ChangeBehavior(string)
  | ChangePayload(string)
  | ClearInputs;

let component = ReasonReact.reducerComponent("WebhookManagementNew");

let make = (~create, _children) => {
  ...component,
  initialState: () => {
    name: "",
    url: "",
    event: "",
    source: "Order",
    behavior: "FireAndForget",
    payload: Webhook.PayloadType.(Json |> toString),
  },
  reducer: (action, state) =>
    switch (action) {
    | ChangeName(newVal) => ReasonReact.Update({...state, name: newVal})
    | ChangeUrl(newVal) => ReasonReact.Update({...state, url: newVal})
    | ChangeEvent(newVal) => ReasonReact.Update({...state, event: newVal})
    | ChangeSource(newVal) => ReasonReact.Update({...state, source: newVal})
    | ChangeBehavior(newVal) =>
      ReasonReact.Update({...state, behavior: newVal})
    | ChangePayload(newVal) =>
      ReasonReact.Update({...state, payload: newVal})
    | ClearInputs => ReasonReact.Update({...state, name: "", url: ""})
    },
  render: self => {
    let finishedEnteringData = () => {
      let newWebhook: Webhook.New.t = {
        name: self.state.name,
        url: self.state.url,
        event: self.state.event |> Webhook.EventType.toT,
        source: self.state.source |> Webhook.EventSource.toT,
        behavior: self.state.behavior |> Webhook.Behavior.fromString,
        payload: self.state.payload |> Webhook.PayloadType.fromString,
      };
      self.send(ClearInputs);
      create(newWebhook);
    };
    <tr>
      <td>
        <input
          value={self.state.name}
          onChange={ev => self.send(ChangeName(getVal(ev)))}
        />
      </td>
      <td>
        <input
          value={self.state.url}
          onChange={ev => self.send(ChangeUrl(getVal(ev)))}
        />
      </td>
      <td>
        <input
          value={self.state.event}
          onChange={ev => self.send(ChangeEvent(getVal(ev)))}
        />
      </td>
      <td>
        <input
          value={self.state.source}
          onChange={ev => self.send(ChangeSource(getVal(ev)))}
        />
      </td>
      <td>
        <input
          value={self.state.behavior}
          onChange={ev => self.send(ChangeBehavior(getVal(ev)))}
        />
      </td>
      <td>
        <input
          value={self.state.payload}
          onChange={ev => self.send(ChangePayload(getVal(ev)))}
        />
      </td>
      <td>
        <button onClick={_ => finishedEnteringData()}>
          {ReactUtils.s("Crear")}
        </button>
      </td>
    </tr>;
  },
};