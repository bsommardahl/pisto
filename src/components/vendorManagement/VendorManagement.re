open Js.Promise;

type state = {vendors: list(Vendor.t)};

type action =
  | LoadVendors(list(Vendor.t))
  | VendorRemoved(Vendor.t)
  | VendorModified(Vendor.t)
  | NewVendorCreated(Vendor.t);

let component = ReasonReact.reducerComponent("VendorManagement");

let make = _children => {
  ...component,
  didMount: self => {
    VendorStore.getAll()
    |> Js.Promise.then_(vendors => {
         self.send(LoadVendors(vendors));
         Js.Promise.resolve();
       })
    |> ignore;
    ();
  },
  initialState: () => {vendors: []},
  reducer: (action, state) =>
    switch (action) {
    | LoadVendors(vendors) => ReasonReact.Update({vendors: vendors})
    | VendorRemoved(exp) =>
      ReasonReact.Update({
        vendors:
          state.vendors |> List.filter((d: Vendor.t) => d.id !== exp.id),
      })
    | VendorModified(exp) =>
      ReasonReact.Update({
        vendors:
          state.vendors
          |> List.map((d: Vendor.t) => d.id !== exp.id ? exp : d),
      })
    | NewVendorCreated(exp) =>
      ReasonReact.Update({vendors: List.concat([state.vendors, [exp]])})
    },
  render: self => {
    let goBack = _ => ReasonReact.Router.push("/admin");
    let removeVendor = (p: Vendor.t) => {
      VendorStore.remove(p.id)
      |> then_(_ => {
           self.send(VendorRemoved(p));
           resolve();
         })
      |> ignore;
      ();
    };
    let modifyVendor = (modifiedVendor: Vendor.t) =>
      VendorStore.update(modifiedVendor)
      |> then_(_ => {
           self.send(VendorModified(modifiedVendor));
           resolve();
         })
      |> ignore;
    let createVendor = (newVendor: Vendor.New.t) => {
      VendorStore.add(newVendor)
      |> Js.Promise.then_((newVendor: Vendor.t) => {
           self.send(NewVendorCreated(newVendor));
           Js.Promise.resolve();
         })
      |> ignore;
      ();
    };
    <div className="admin-menu">
      <div className="header">
        <div className="header-menu">
          <Button
            className="quiet-card"
            onClick=goBack
            local=true
            label="nav.back"
          />
        </div>
        <div className="header-options">
          (ReactUtils.sloc("admin.vendors.header"))
        </div>
      </div>
      <div className="vendor-management">
        <table className="table">
          <thead>
            <tr> <th /> <th> (ReactUtils.sloc("vendor.name")) </th> </tr>
          </thead>
          <tbody>
            (
              self.state.vendors
              |> List.map((d: Vendor.t) =>
                   <VendorManagementRow
                     vendor=d
                     remove=removeVendor
                     modify=modifyVendor
                     key=d.id
                   />
                 )
              |> Array.of_list
              |> ReasonReact.array
            )
          </tbody>
          <tfoot> <VendorManagementNew create=createVendor /> </tfoot>
        </table>
      </div>
    </div>;
  },
};