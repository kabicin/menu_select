package dynastyhouse.menuselect;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;


public class ResourceCentreActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_resource_centre);
    }

    /**
     * onClick listener for the Resource Centre buttons
     *
     * @param v the button being clicked
     */
    public void redirect(View v){
        Intent i;
        switch(v.getId()){
            case R.id.buttonOrder:
                i = new Intent(ResourceCentreActivity.this, OrderActivity.class);
                startActivity(i);
                break;

            case R.id.buttonMenu:
                i = new Intent(ResourceCentreActivity.this, MenuActivity.class);
                startActivity(i);
                break;

            case R.id.buttonSettings:
                i = new Intent(ResourceCentreActivity.this, SettingsActivity.class);
                startActivity(i);
                break;
        }

    }

}
