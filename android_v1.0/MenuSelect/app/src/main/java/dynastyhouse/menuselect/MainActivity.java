package dynastyhouse.menuselect;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    EditText username;
    EditText password;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        username = findViewById(R.id.editTextUsername);
        password = findViewById(R.id.editTextPassword);
    }

    /**
     * onClick listener for the login button
     *
     * @param v The current view
     */
    public void tryLogin(View v) {
        if (validateLogin()) {
            Toast.makeText(this, "Successfully logged in!", Toast.LENGTH_SHORT).show();
            Intent i = new Intent(MainActivity.this, ResourceCentreActivity.class);
            startActivity(i);
        } else {
            Toast.makeText(this, "Invalid username or password...", Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * Validates login information with admin login credentials (for debugging)
     *
     * @return true if the user credentials are correct, otherwise false.
     */
    private boolean validateLogin() {
        return true;
        /* if (username == null || password == null)
            return false;
        return username.getText().toString().equals("admin") && password.getText().toString().equals("123");
        */
    }
}
